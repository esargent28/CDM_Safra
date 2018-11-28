/* ************************************************************************** *
 *                                                                            *
 *                15-354: Computational Discrete Mathematics                  *
 *                     Final Project: Safra's Algorithm                       *
 *             Erik Sargent (esargent), Vaidehi Srinivas (vaidehis)           *
 *                                                                            *
 *             main.cpp - contains main logic for Safra's algorithm           *
 *                                                                            *
 * ************************************************************************** */

#include "safra_tree.h"

#include <iostream>
#include <cstdint>

// ============= Helper methods to implement Safra's algorithm ============== //





// ==================== Main method for Safra's algorithm =================== //

int main(int argc, const char *argv[]) {

    std::cout << "# args: " << argc << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < argc; i++) {
        std::cout << "Argument #" << i << ": " << argv[i] << std::endl;
    }

    return 0;
}