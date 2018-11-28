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

/*
 *  Overall algorithm logic: 
 *  
 *  T0 = initial Safra tree based on automaton input
 *  tree_set = {T0}
 *  transition_set = {}
 *  tree_queue = [T0]
 *  deterministic automaton has state T
 *
 *  while tree queue is not empty:
 *      T = tree_queue.front();
 *      for c in alphabet:
 *          T' = safra_transition(T, c)
 *          if (T' not in tree_set):
 *              tree_set.insert(T')
 *              tree_queue.push(T')
 *          transition_set.insert(T,c -> T')
 *  
 *  Determine Rabin pairs based on trees in tree_set
 *  Return automaton based on tree_set, transition_set, and Rabin pairs
 */


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