//  ----------------------------------------------------------------------------
//  Main file of the Standalone solver.                                 main.cpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on October 23, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#include "driver.hpp"
#include "tools.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    banner();
    microsat::Driver driver(argv[1]);
    driver.run();
    bye();
    return 0;
}