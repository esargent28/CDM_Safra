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
#include <fstream>
#include <cstdint>

#include <string.h>

// Definitions & constants for I/O purposes
#define BUFFER_SIZE 100

#define NUM_STATES_TAG "# Number of states"
#define ALPHABET_SIZE_TAG "# Alphabet size"
#define NUM_TRANSITIONS_TAG "# Number of transitions"
#define BEGIN_TRANSITIONS_TAG "# begin transitions"
#define END_TRANSITIONS_TAG "# end transitions"
#define INITIAL_STATES_TAG "# Buechi initial"
#define FINAL_STATES_TAG "# Buechi final"
#define EOF_TAG "# Buechi eof"

/*
 *  Overall algorithm logic: 
 *  
 *  T0 = initial Safra tree based on automaton input
 *  tree_set = { T0 }
 *  transition_set = { }
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

    SafraTree *t = new SafraTree();

    std::fstream infile, outfile;
    char buffer[100];

    if (argc != 3) {
        std::cout << "ERROR: Incorrect argument format. ";
        std::cout << "Usage: ./safra <ipnutfile> <outputfile>  ";
        std::cout << "(file format in info.txt)" << std::endl;
        return 1;
    }

    std::cout << "Extracting Buechi automaton from file " << argv[1];
    std::cout << "..." << std::endl;

    // ========================= PROCESS INPUT FILE ========================= //

    infile.open(argv[1], std::ios::in);
    if (!infile.is_open()) {
        std::cout << "ERROR: Improper input filename." << std::endl;
        return 1;
    }

    while (!infile.fail()) {
        infile.getline(buffer, BUFFER_SIZE, '\n');
        std::cout << buffer << std::endl;



    }

    infile.close();

    std::cout << "Extraction done. Running Safra's algorithm..." << std::endl;

    // ======================= RUN SAFRA'S ALGORITHM ======================== //






    // ======================== PROCESS OUTPUT FILE ========================= //

    std::cout << "Safra's algorithm done. Writing result to file " << argv[2];
    std::cout << "..." << std::endl;

    // Open output file
    outfile.open(argv[2], std::ios::out);
    if (!outfile.is_open()) {
        std::cout << "ERROR: Improper output filename." << std::endl;
        return 1;
    }

    // Close output file
    outfile.close();

    return 0;
}