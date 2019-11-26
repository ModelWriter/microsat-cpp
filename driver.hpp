//  ----------------------------------------------------------------------------
//  Header file for the driver class.                                 driver.hpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on November 26, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef MICROSAT_DRIVER_HPP
#define MICROSAT_DRIVER_HPP

#include "solver.hpp"
#include <utility>

namespace microsat {

class driver {
  private:
    const std::string filename;
    std::unique_ptr<Solver> solver = nullptr;
    int parse();

  public:
    explicit driver(std::string file);

    static void instructions();
};

} // namespace microsat

#endif // MICROSAT_DRIVER_HPP
