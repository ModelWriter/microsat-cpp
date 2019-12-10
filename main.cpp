//  ----------------------------------------------------------------------------
//  Main file of the Standalone solver.                                 main.cpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on October 23, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#include "driver.hpp"
#include "tools.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// initiates the driver with the supplied DIMACS filename
void run(int argc, char* argv[]) {
    bool stats = false;
    std::string filename;
    if (argc > 5)
        throw Fatal("Invalid number of arguments: %i\n", argc - 1);
    for (int i = 1; i < argc; ++i)
        if (argv[i] == std::string("-h") || argv[i] == std::string("--help")) {
            microsat::driver::instructions();
            return;
        } else if (argv[i] == std::string("-s") ||
                   argv[i] == std::string("--stats")) {
            stats = true;
        } else if (argv[i] == std::string("-f")) {
            filename = argv[++i];
        } else {
            microsat::driver::instructions();
            throw Fatal("Invalid Parameter: %s\n", argv[i]);
        }
    microsat::driver drv(filename, stats);
}

// -----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    P(banner());
    try {
        run(argc, argv);
    } catch (Fatal& e) {
        std::cout << e.what() << "\n";
    } catch (std::runtime_error& e) {
        std::cerr << "\n\nCatching Runtime Error...\n\n" << e.what() << "\n";
    } catch (std::bad_alloc& e) { // handle memory exhaustion
        std::cerr << "\n\nCatching Allocation error...\n\n" << e.what() << "\n";
        return 1; // exit main
    } catch (...) {
        std::cout << "Uncaught exception..." << std::endl;
        throw;
    }
    P(bye());
    return 0;
}
