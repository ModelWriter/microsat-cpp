//  ----------------------------------------------------------------------------
//  Implementation file for the solver class.                         solver.cpp
//  The MIT License
//  Copyright (c) 2014-2018 Marijn Heule (C Version)
//  Modified by Ferhat Erata <ferhat.erata@yale.edu> on November 26, 2018.
// -----------------------------------------------------------------------------

#include "solver.hpp"
#include "tools.hpp"

using namespace microsat;

// -----------------------------------------------------------------------------
// Default constructor that initializes the data structures
Solver::Solver(int n, int m) : nVars(n), nClauses(m), mem(mem_max) {
    db = mem.get_raw_memory();
    v_model.reserve(n + 1);       // Full assignment of the variables
    v_next.reserve(n + 1);        // Next variable in the heuristic order
    v_prev.reserve(n + 1);        // Previous variable in the heuristic order
    v_buffer.reserve(n);          // A buffer to store a temporary clause
    v_reason.reserve(n + 1);      // Array of clauses
    v_false_stack.reserve(n + 1); // Stack of falsified literals

    model = v_model.data();
    next = v_next.data();
    prev = v_prev.data();
    buffer = v_buffer.data();
    reason = v_reason.data();
    false_stack = v_false_stack.data();

    forced = false_stack;    // Points inside *falseStack at first decision
    processed = false_stack; // Points inside *falseStack at first unprocessed
    assigned = false_stack;  // Points inside *falseStack at last unprocessed

    // Labels for variables, non-zero means false
    false_ = mem.allocate(2 * n + 1) + n; // having both polarity
    // Offset of the first watched clause
    first = mem.allocate(2 * n + 1) + n; // having both polarity
    // Make sure there is a 0 before the clauses are loaded.
    *mem.allocate(1) = 0;

    // Initialize the main data structures:
    for (int i = 1; i <= n; i++) { // for each variable
        // variable selection: which variable to assign next?
        prev[i] = i - 1;
        next[i - 1] = i; // the double-linked list for variable-move-to-front,
        // phase selection: assign variable to which phase (true or false)?
        model[i] = 0;               // the model (phase-saving)
        false_[-i] = false_[i] = 0; // the false array,
        first[i] = first[-i] = END; // and first (watch pointers).
    }
    // decision heuristics
    head = n; // Initialize the head of the double-linked list
}

// -----------------------------------------------------------------------------
// Adds a clause stored in *in of size size
int* Solver::addClause(int* in, int size, int irr) {
    // Store a pointer to the beginning of the clause
    int clause_head = mem_used();
    // Allocate memory for the clause in the database
    int* clause = mem.allocate(size + 2 + 1);
    // shift the head of the clause because the first two for watch pointers
    clause += 2;
    // If the clause is not unit, then add two watch pointers to the data
    // structure
    if (size > 1) { // first two literal are watched
        addWatch(in[0], clause_head);
        addWatch(in[1], clause_head + 1);
    }
    // Copy the clause from the buffer to the database
    std::copy(in, in + size, clause);
    clause[size] = 0;
    // Update the statistics
    if (irr)
        mem_fixed = mem_used();
    else
        nLemmas++;
    // Return the pointer to the clause in the database
    return clause;
}

// -----------------------------------------------------------------------------
// Adds a watch pointer to a clause containing lit
void Solver::addWatch(int literal, int mem) {
    db[mem] = first[literal]; // By updating the database and the pointers
    first[literal] = mem;
}

// -----------------------------------------------------------------------------
// Unassign the literal
void Solver::unassign(int lit) { false_[lit] = 0; }

// -----------------------------------------------------------------------------
// Perform a restart (i.e., unassign all variables)
void Solver::restart() {
    // Remove all unforced false literals from falseStack
    while (assigned > forced)
        unassign(*(--assigned));
    // Reset the processed pointer
    processed = forced;
}

// -----------------------------------------------------------------------------
// Make the first literal of the reason true
void Solver::assign(const int* reason_, int forced_) {
    // Let lit be the first literal in the reason
    int lit = reason_[0];
    // Mark lit as true and IMPLIED if forced
    false_[-lit] = forced_ ? IMPLIED : 1;
    // Push it on the assignment stack
    *(assigned++) = -lit;
    // Set the reason clause of lit
    reason[std::abs(lit)] = 1 + (int)((reason_)-db);
    // Mark the literal as true in the model
    model[std::abs(lit)] = (lit > 0);
}

// -----------------------------------------------------------------------------
// Move the variable to the front of the decision list
// bump variables occurring in learned clauses (strategy is to bump all
// variables used to derive learned clause)
// * Siege SAT solver [Ryan’04] used variable move to front (VMTF)
// * bumped variables moved to head of doubly linked list
// * search for unassigned variable starts at head
void Solver::bump(int literal) {
    if (false_[literal] != IMPLIED) {
        // MARK the literal as involved if not a top-level unit
        false_[literal] = MARK;
        int var = std::abs(literal);
        // In case var is not already the head of the list
        if (var != head) {
            prev[next[var]] = prev[var]; // Update the prev link, and
            next[prev[var]] = next[var]; // Update the next link, and
            next[head] = var;            // Add a next link to the head, and
            prev[var] = head;            // Make var the new head
            head = var;
        }
    }
}

// -----------------------------------------------------------------------------
// Check if literal is implied by MARK literals
int Solver::implied(int literal) {
    // If checked before return old result
    if (false_[literal] > MARK)
        return (false_[literal] & MARK);
    if (!reason[std::abs(literal)])
        return 0; // In case literal is a decision, it is not implied
    int* p = (db + reason[std::abs(literal)] - 1); // Get the reason of literal
    // While there are literals in the reason, recursively check if non-MARK
    // literals are implied
    while (*(++p))
        if ((false_[*p] ^ MARK) && !implied(*p)) {
            false_[literal] = IMPLIED - 1;
            return 0; // Mark and return not implied (denoted by IMPLIED - 1)
        }
    false_[literal] = IMPLIED;
    return 1; // Mark and return that the literal is implied
}

// -----------------------------------------------------------------------------
// Removes "less useful" lemmas from DB
// Remove strategy
void Solver::reduceDB(int k) {
    // Allow more lemmas in the future
    while (nLemmas > maxLemmas)
        maxLemmas += 300;
    // Reset the number of lemmas
    nLemmas = 0;
    // Loop over the variables
    for (int i = -nVars; i <= nVars; i++) {
        if (i == 0)
            continue;
        // Get the pointer to the first watched clause
        int* watch = &first[i];
        while (*watch != END)       // As long as there are watched clauses
            if (*watch < mem_fixed) // Remove the watch if it points to a lemma
                watch = (db + *watch);
            else // Otherwise (meaning an input clause) go to next watch
                *watch = db[*watch];
    }
    // Virtually remove all lemmas
    int old_used = mem_used();
    mem.resize(mem_fixed);
    // While the old memory contains lemmas
    for (int i = mem_fixed + 2; i < old_used; i += 3) {
        // Get the lemma to which the head is pointing
        int count = 0, head_ = i;
        // Count the number of literals
        while (db[i]) {
            int literal = db[i++];
            // That are satisfied by the current model
            if ((literal > 0) == model[std::abs(literal)])
                count++;
        }
        // If the latter is smaller than k, add it back
        if (count < k) {
            auto ref = db + head_;
            addClause(ref, i - head, 0); // ToDo addClause
        }
    }
}

// -----------------------------------------------------------------------------
// Compute a resolvent from falsified clause
int* Solver::analyze(int* clause) {
    // Bump restarts and update the statistic
    res++;
    nConflicts++;
    // MARK all literals in the falsified clause
    while (*clause)
        bump(*(clause++)); // ToDo bump
    // Loop on variables on falseStack until the last decision
    while (reason[std::abs(*(--assigned))]) {
        // If the tail of the stack is MARK
        if (false_[*assigned] == MARK) {
            // Pointer to check if first-UIP is reached
            int* check = assigned;
            // Check for a MARK literal before decision
            while (false_[*(--check)] != MARK)
                // Otherwise it is the first-UIP so break
                if (!reason[std::abs(*check)])
                    goto build;
            // Get the reason and ignore first literal
            clause = db + reason[std::abs(*assigned)];
            // MARK all literals in reason
            while (*clause)
                bump(*(clause++)); // ToDo bump
        }
        // Unassign the tail of the stack
        unassign(*assigned); // ToDo unassign
    }

// Build conflict clause; Empty the clause buffer
build:;
    int size = 0;
    int lbd = 0; // Literal Block Distance (LBD) of Glucose
    int flag = 0;
    // Loop from tail to front
    int* p = processed = assigned;
    // Only literals on the stack can be MARKed
    while (p >= forced) {
        // If MARKed and not implied
        if ((false_[*p] == MARK) && !implied(*p)) { // ToDo implied
            // Add literal to conflict clause buffer
            buffer[size++] = *p;
            flag = 1;
        }
        // Increase LBD for a decision with a true flag
        if (!reason[std::abs(*p)]) {
            lbd += flag;
            flag = 0;
            // And update the processed pointer
            if (size == 1)
                processed = p;
        }
        // Reset the MARK flag for all variables on the stack
        false_[*(p--)] = 1;
    }

    // glucose level (LBD) of learned clause:
    // * number of different decision levels in a learned clauses
    // * calculated at the point the clause is learned during conflict analysis

    // Update the fast moving average
    fast -= fast >> 5;
    fast += lbd << 15;
    // Update the slow moving average
    slow -= slow >> 15;
    slow += lbd << 5;

    // Loop over all unprocessed literals
    while (assigned > processed)
        // Unassign all lits between tail & head
        unassign(*(assigned--)); // ToDo unassign
    // Assigned now equal to processed
    unassign(*assigned); // ToDo unassign
    // Terminate the buffer (and potentially print clause)s
    buffer[size] = 0;
    // Add new conflict clause to redundant db
    return addClause(buffer, size, 0); // ToDo addClause
}

// -----------------------------------------------------------------------------
// Performs unit propagation
int Solver::propagate() {
    // Initialize forced flag
    int forced_ = reason[std::abs(*processed)];
    // While unprocessed false literals
    while (processed < assigned) {
        // Get first unprocessed literal
        int lit = *(processed++);
        // Obtain the first watch pointer
        int* watch = &first[lit];
        // While there are watched clauses (watched by lit)
        while (*watch != END) { // while (*watch != -9)
            // Let's assume that the clause is unit
            bool unit = true;
            // Get the clause from db
            int* clause = (db + *watch + 1); // it might be the first or second
            // Set the pointer to the first literal in the clause
            // if this is the second watch literal, then forward the pointer...
            if (clause[-2] == 0) // all clauses ends with a 0... even there is
                clause++;        // a zero before the first clause....
            // Ensure that the other watched literal is in front
            if (clause[0] == lit)
                clause[0] = clause[1];
            // Scan the non-watched literals
            for (int i = 2; unit && clause[i]; i++)
                // When clause[i] is not false, it is either true or unset
                if (!false_[clause[i]]) {
                    // Swap literals
                    clause[1] = clause[i];
                    clause[i] = lit;
                    int store = *watch; // Store the old watch
                    unit = false;
                    // Remove the watch from the list of lit
                    *watch = db[*watch];
                    addWatch(clause[1], store); // ToDo addWatch
                }       // Add the watch to the list of clause[1]
            if (unit) { // If the clause is indeed unit
                clause[1] = lit;
                // Place lit at clause[1] and update next watch
                watch = (db + *watch);
                // If the other watched literal is satisfied continue
                if (false_[-clause[0]])
                    continue;
                // If the other watched literal is falsified,
                if (!false_[clause[0]]) {
                    // A unit clause is found, and the reason is set
                    assign(clause, forced_); // ToDo assign
                } else {
                    // Found a root level conflict -> UNSAT
                    if (forced_)
                        return UNSAT;
                    // Analyze the conflict return a conflict clause
                    int* lemma = analyze(clause); // ToDo analyze
                    // In case a unit clause is found, set forced flag
                    if (!lemma[1])
                        forced_ = 1;
                    assign(lemma, forced_); // ToDo assign
                    break;
                }
            }
        } // end of while (*watch != END)
    }
    // Assign the conflict clause as a unit
    if (forced_)
        forced = processed; // Set forced if applicable
    return SAT;             // Finally, no conflict was found
}

// -----------------------------------------------------------------------------
// determines satisfiability
int Solver::solve() {
    // Initialize the solver
    int decision = head;
    res = 0;
    // Main solve loop
    for (;;) {
        // Store nLemmas to see whether propagate adds lemmas
        int old_nLemmas = nLemmas;
        // Propagation returns UNSAT for a root level conflict
        if (propagate() == UNSAT) // may generate a lemma through analyze()...
            return UNSAT;
        // If the last decision caused a conflict
        if (nLemmas > old_nLemmas) {
            // Reset the decision heuristic to head
            decision = head;
            // If fast average is substantially larger than slow average
            if (restarting()) {
                printf("c restarting after %i conflicts (%i %i) %i\n", res,
                       fast, slow, nLemmas > maxLemmas);
                // Restart and update the averages
                res = 0;
                fast = (slow / 100) * 125;
                restart(); // ToDo restart
                // Reduce the DB when it contains too many lemmas
                // Remove "less useful" lemmas from DB
                if (nLemmas > maxLemmas)
                    reduceDB(6); // ToDo reduceDB
            }
        }
        // As long as the temporary decision is assigned
        while (false_[decision] || false_[-decision]) {
            // Replace it with the next variable in the decision list
            decision = prev[decision];
        }
        // If the end of the list is reached, then a solution is found
        if (decision == 0)
            return SAT;
        // Otherwise, assign the decision variable based on the model
        decision = model[decision] ? decision : -decision;
        // Assign the decision literal to true (change to IMPLIED-1?)
        false_[-decision] = 1;
        // And push it on the assigned stack
        *(assigned++) = -decision;
        // Decisions have no reason clauses
        decision = std::abs(decision);
        reason[decision] = 0;
    }
}

// -----------------------------------------------------------------------------
//
bool Solver::restarting() { return fast > (slow / 100) * 125; }
