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
    if (argc != 2)
        throw Fatal("Invalid number of arguments: %i\n", argc - 1);
    std::string str;
    std::stringstream ss(argv[1]);
    ss >> str; // Try to get the following argument
    if (ss.fail())
        throw Fatal("Invalid Parameter: %s\n", argv[1]);
    if ((str == "-h") || (str == "--help"))
        microsat::driver::instructions();
    microsat::driver drv(str);
}

// -----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    P(banner());
    try {
        run(argc, argv);
    } catch (Fatal& e) {
        std::cout << "\n\nCatching Fatal exception...\n\n" << e.what() << "\n";
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
