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
Driver::Driver(std::string filename) : filename(std::move(filename)) {
    std::ifstream in(filename);
    if (in) { // checks if the file is open

    }
}

// -----------------------------------------------------------------------------
//
void Driver::run() {}
