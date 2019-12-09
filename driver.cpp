//  ----------------------------------------------------------------------------
//  Implementation file for the driver class.                         driver.cpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on November 26, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#include "driver.hpp"

#include "tools.hpp"
#include <utility>

using namespace microsat;

// -----------------------------------------------------------------------------
//
driver::driver(std::string file) : filename(std::move(file)) {
    if (parse() == UNSAT) {
        std::cout << "unsat";
    }
    // Solve without limit (number of conflicts)
    else if (solver->solve() == UNSAT) {
        std::cout << "unsat";
    }
    // And print whether the formula has a solution
    else {
        std::cout << "sat ";
        for (int i = 1; i <= solver->nVars; i++) {
            if (solver->model[i])
                std::cout << i << " ";
            else
                std::cout << "-" << i << " ";
        }
        std::cout << "\n";
    }
    // Print the statistics

    P("c--------------------------------------------------------------\n"
      << "c statistics of " << filename << ":\n"
      << "c [ mem_used: " << solver->mem_used() << ", conflicts: "
      << solver->nConflicts << ", lemmas: " << solver->nLemmas
      << ", max_lemmas: " << solver->maxLemmas << " ]");
}

// -----------------------------------------------------------------------------
// Parse the DIMACS file
int driver::parse() {
    std::ifstream in(filename);
    if (in) { // checks if the file is open
        string line;
        while (!in.eof()) {
            getline(in, line);
            if (line.length() == 0 || line[0] == 'c') {
                continue;
            } else if (line[0] == 'p') {
                std::size_t pos = line.find("cnf", 0);
                if (pos != std::string::npos) {
                    std::string str = line.substr(pos + 4, line.length());
                    int nVars, nClauses;
                    std::istringstream ss(str);
                    if (!(ss >> nVars >> nClauses)) // extract vars and clausesS
                        throw Fatal("can't extract nVars and nClauses!");
                    P("c p cnf " << nVars << " " << nClauses);
                    // late binding of the solver
                    solver = std::make_unique<Solver>(nVars, nClauses);
                }
            } else {
                std::istringstream ss(line);
                auto& s = *solver;
                int size = 0, literal = 0;
                P("c ");
                do {                     // for each clause in the file
                    if (ss >> literal) { // !in.fail()
                        if (literal == 0)
                            break;
                        s.buffer[size++] = literal;
                        P(literal << " ");
                    }
                } while (ss.good());
                P("\n");
                if (!ss.eof())
                    throw Fatal(
                        "I/O error or bad data during clause extraction");
                // reached the end of the clause; add the clause to database
                int* clause = s.addClause(s.buffer, size, 1);
                // Check for empty clause or conflicting unit
                // If either is found return UNSAT
                if (!size || ((size == 1) && s.false_[clause[0]]))
                    return UNSAT; //
                // Check for a new unit
                if ((size == 1) && !s.false_[-clause[0]]) {
                    s.assign(clause, 1);
                } // Directly assign new units (forced = 1)
            }     // Reset buffer
        }

    } // End While
    in.close();
    return SAT; // Return that no conflict was observed
}

// -----------------------------------------------------------------------------
// driver instructions
void driver::instructions() {
    std::cout << "\nUsage: microsat++ <options>"
                 "\n\nOption(s):\n"
                 "\t-h,--help\tShow this help message\n"
                 "\t-f <file>\tDIMACS cnf file"
              << std::endl;
}
