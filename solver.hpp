//  ----------------------------------------------------------------------------
//  Header file for the solver class.                                 solver.hpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on November 25, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef MICROSAT_SOLVER_HPP
#define MICROSAT_SOLVER_HPP

#include <memory>

constexpr int mem_max = 1u << 20u; // the initial maximum memory
constexpr int ave_max = 1u << 24u; // the initial average

namespace microsat {

// ------------------------------------------------------------------------
enum { END = -9, UNSAT = 0, SAT = 1, MARK = 2, IMPLIED = 6 };
// -----------------------------------------------------------------------------
class Solver {
    friend class driver;

  private:
    const int nVars;      // The number of variables
    const int nClauses;   // The number of clauses
    int mem_used = 0;     // The number of integers allocated in the DB
    int nLemmas = 0;      // The number of learned (redundant) clauses
    int nConflicts = 0;   // Under of conflicts which is used to updates scores
    int maxLemmas = 2000; // Initial maximum number of learnt clauses
    int fast = ave_max;   // Initialize the fast moving averages
    int slow = ave_max;   // Initialize the slow moving averages
    int mem_fixed = 0;    // ?
    // ------------------------------------------------------------------------
    // initial database
    std::unique_ptr<int[]> db = nullptr;
    // ------------------------------------------------------------------------
    int* model = nullptr;  // Full assignment of the variables
                           // (initially false)
    int* prev = nullptr;   // Previous variable in the heuristic order
    int* next = nullptr;   // Next variable in the heuristic order
    int* buffer = nullptr; // A buffer to store a temporary clause
    int* reason = nullptr; // Array of clauses
    // ------------------------------------------------------------------------
    int* falseStack = nullptr; // Stack of falsified literals; never changes
    int* forced = nullptr;     // Points inside *falseStack at first decision
                               // (unforced literal)
    int* processed = nullptr;  // Points inside *falseStack at first unprocessed
                               // literal
    int* assigned = nullptr;   // Points inside *falseStack at last unprocessed
                               // literal
    // ------------------------------------------------------------------------
    int* false_ = nullptr; // Labels for variables, non-zero means false
    int* first = nullptr;  // Offset of the first watched clause
    int head = 0;          // the head of the double-linked list
    int res = 0;           // restart counter ??
    // ------------------------------------------------------------------------
    int* getMemory(int mem_size); // Allocate memory of size mem_size

  public:
    // The code assumes that there is at least one variable
    explicit Solver(int vars = 1, int clauses = 0);
    // Adds a clause stored in *in of size size
    int* addClause(int* in, int size, int irr);
    // Adds a watch pointer to a clause containing lit
    void addWatch(int literal, int mem);
    // Perform a restart (i.e., unassign all variables)
    void restart();
    // Unassign the literal
    void unassign(int literal);
    // assign the literal
    void assign(int* reason, int forced);
    // Move the variable to the front of the decision list
    void bump(int literal);
    // Check if literal is implied by MARK literals
    int implied(int literal);
    // Compute a resolvent from falsified clause
    int* analyze(int* clause);
    // Performs unit propagation
    int propagate();
    // determines satisfiability
    int solve();
    // Removes "less useful" lemmas from DB
    void reduceDB(int k);
};

} // namespace microsat
#endif // MICROSAT_SOLVER_HPP
