/* ************************************************************************** *
 *                                                                            *
 *                15-354: Computational Discrete Mathematics                  *
 *                     Final Project: Safra's Algorithm                       *
 *             Erik Sargent (esargent), Vaidehi Srinivas (vaidehis)           *
 *                                                                            *
 *         main.cpp - contains I/O and main logic for Safra's algorithm       *
 *                                                                            *
 * ************************************************************************** */

#include "safra_tree.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdint>
#include <iomanip>

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

// Buffer to hold 
char buffer[100];

// Input & output file streams
std::fstream infile, outfile;

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


// ============== Helper methods for reading Buechi automaton =============== //

/*
 * Constructs the transition vector based on the given num_states and
 *   alphabet_size
 */
void InitializeTransitionVector(const int &num_states, const int &alphabet_size,
    std::vector<std::vector<int64_t>> &transitions) {

    for (int state = 0; state < num_states; state++) {
        std::vector<int64_t> v;
        for (int character = 0; character < alphabet_size; character++) {
            v.push_back(0);
        }
        transitions.push_back(v);
    }
}


/*
 * Insert a transition from state #pre to state #post along character
 */
void InsertTransition(const int &pre_state, const int &post_state,
    const int &character, std::vector<std::vector<int64_t>> &transitions) {

    transitions[pre_state][character] |= (1 >> post_state);
}


/*
 * Read the (open) input file stream and populate the information about the
 *   Buechi automaton to the provided arugments. Returns true if the read
 *   produced a full, valid Buechi automaton, and false otherwise.
 */
bool ReadBeuchi(int &num_states, int &alphabet_size, int64_t &initial_states,
    int64_t &final_states, std::vector<std::vector<int64_t>> &transitions) {

    // Possible read states we can be in
    enum ReadState {
        INVALID,
        WAIT_FOR_TAG,
        READ_NUM_STATES,
        READ_ALPHABET_SIZE,
        READ_NUM_TRANSITIONS,
        READ_TRANSITIONS,
        READ_INITIAL_STATES,
        READ_FINAL_STATES,
        DONE
    };

    // Ensure that all fields start as invalid fields
    bool found_num_states = false, found_alphabet_size = false,
         found_initial_states = false, found_transitions = false,
         found_num_transitions = false, found_final_states = false;

    num_states = -1;
    alphabet_size = -1;
    initial_states = 0;
    final_states = 0;
    transitions.clear();

    // Running count of the # of transitions we've read
    int num_transitions = -1;
    int transition_count = 0;

    ReadState state = WAIT_FOR_TAG;

    while (state != INVALID && state != DONE) {

        infile.getline(buffer, BUFFER_SIZE, '\n');
        std::string line(buffer);
        std::stringstream linestream(line);

        switch (state) {

            // WAIT_FOR_TAG:
            //   If we find a proper tag, switch to corresponding state
            case (WAIT_FOR_TAG):
                if (!line.compare(NUM_STATES_TAG)) {
                    state = (found_num_states ? INVALID : READ_NUM_STATES);
                }
                else if (!line.compare(ALPHABET_SIZE_TAG)) {
                    state = (found_alphabet_size ? INVALID : READ_ALPHABET_SIZE);
                }
                else if (!line.compare(NUM_TRANSITIONS_TAG)) {
                    state = (found_num_transitions ? INVALID : READ_NUM_TRANSITIONS);
                }
                else if (!line.compare(BEGIN_TRANSITIONS_TAG)) {
                    if (!found_transitions && found_num_states &&
                        found_num_transitions && found_alphabet_size) {
                        state = READ_TRANSITIONS;
                    }
                    else { state = INVALID; }
                }
                else if (!line.compare(INITIAL_STATES_TAG)) {
                    state = (found_initial_states ? INVALID : READ_INITIAL_STATES);
                }
                else if (!line.compare(FINAL_STATES_TAG)) {
                    state = (found_final_states ? INVALID : READ_FINAL_STATES);
                }
                else if (!line.compare(EOF_TAG)) {
                    if (found_num_states && found_alphabet_size &&
                        found_initial_states && found_transitions &&
                        found_num_transitions && found_final_states) {
                        state = DONE;
                    }
                    else { state = INVALID; }
                }
                else if (!line.compare(END_TRANSITIONS_TAG)) { state = INVALID; }
                break;

            // READ_NUM_STATES:
            //   Parse a single number, set num_states, wait for new tag
            case (READ_NUM_STATES):
                if (found_num_states) { state = INVALID; }
                else {
                    found_num_states = true;
                    linestream >> num_states;
                    state = (num_states > 0 ? WAIT_FOR_TAG : INVALID);

                    // If we have both num_states and alphabet_size, construct
                    //   our transition vector
                    if (alphabet_size > 0 && num_states > 0) {
                        InitializeTransitionVector(num_states, alphabet_size, transitions);
                    }
                }
                break;

            // READ_ALPHABET_SIZE:
            //   Parse a single number, set alphabet_size, wait for new tag
            case (READ_ALPHABET_SIZE):
                if (found_alphabet_size) { state = INVALID; }
                else {
                    found_alphabet_size = true;
                    linestream >> alphabet_size;
                    state = (alphabet_size >= 0 ? WAIT_FOR_TAG : INVALID);

                    // If we have both num_states and alphabet_size, construct
                    //   our transition vector
                    if (alphabet_size > 0 && num_states >= 0) {
                        InitializeTransitionVector(num_states, alphabet_size, transitions);
                    }
                }
                break;

            // READ_NUM_TRANSITIONS:
            //   Parse a single number, set num_transitions, wait for new tag
            case (READ_NUM_TRANSITIONS):
                if (found_num_transitions) { state = INVALID; }
                else {
                    found_num_transitions = true;
                    linestream >> num_transitions;
                    state = (num_transitions > 0 ? WAIT_FOR_TAG : INVALID);
                }
                break;

            // READ_TRANSITIONS:
            //   If we reach the end tag, check the # of transitions we've found
            //     and compare it to the file's num_transitions. Otherwise, read
            //     three integers and insert a transitions, incrementing the
            //     running transition count
            case (READ_TRANSITIONS):
                if (!line.compare(END_TRANSITIONS_TAG)) {
                    if (transition_count == num_transitions) {
                        found_transitions = true;
                        state = WAIT_FOR_TAG;
                    }
                    else { state = INVALID; }
                }
                else {
                    int pre_state = -1, character = -1, post_state = -1;
                    linestream >> pre_state;
                    linestream >> character;
                    linestream >> post_state;

                    if (pre_state > 0 && pre_state <= num_states &&
                        character > 0 && character <= alphabet_size &&
                        post_state > 0 && post_state <= num_states) {
                        // Switch from 1-indexing to 0-indexing
                        InsertTransition(--pre_state, --character, --post_state, transitions);
                    }
                    else { state = INVALID; }
                    transition_count++;
                }
                break;

            // READ_INITIAL_STATES:
            //   While there's still a number left to parse, add a state into
            //   the initial_states bitvector
            case (READ_INITIAL_STATES):
                if (found_initial_states || !found_num_states) { state = INVALID; }
                else {
                    int i = -1;
                    linestream >> i;
                    while (i > 0 && i <= num_states) {
                        i--; //switch from 1-indexing to 0-indexing
                        initial_states |= (1 << i);
                        i = -1;
                        linestream >> i;
                    }
                    found_initial_states = true;
                    state = WAIT_FOR_TAG;
                }
                break;

            // READ_FINAL_STATES:
            //   While there's still a number left to parse, add a state into
            //   the final_states bitvector
            case (READ_FINAL_STATES):
                if (found_final_states || !found_num_states) { state = INVALID; }
                else {
                    int i = -1;
                    linestream >> i;
                    while (i > 0 && i <= num_states) {
                        i--; // switch from 1-indexing to 0-indexing
                        final_states |= (1 << i);
                        i = -1;
                        linestream >> i;
                    }
                    found_final_states = true;
                    state = WAIT_FOR_TAG;
                }
                break;

            default:
                break;
        }

        if (infile.fail()) { state = INVALID; }
    }
    return (state == DONE);
}

// ============== Helper methods for running Safra's Algorithm ============== //

/*
 * Runs Safra's algorithm on the provided Buechi automaton.
 */
void RunSafra() {

}


// ====== Helper methods for writing Rabin automaton to output stream ======= //

/*
 * Writes the contents of the computed Rabin automaton to the specified output
 *   file stream.
 */
void WriteRabin() {

    outfile << "Placeholder output" << std::endl;
}


// ==================== Main method for Safra's algorithm =================== //

int main(int argc, const char *argv[]) {

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

    int num_states, alphabet_size;
    int64_t initial_states, final_states;
    std::vector<std::vector<int64_t>> transitions;

    if (!ReadBeuchi(num_states, alphabet_size, initial_states,
        final_states, transitions)) {

        std::cout << "Error: Improperly formatted input file. ";
        std::cout << "Please look to info.txt for input file format.\n";
        return 1;
    }

    infile.close();

    // ======================= RUN SAFRA'S ALGORITHM ======================== //

    std::cout << "Extraction done. Running Safra's algorithm..." << std::endl;

    RunSafra();    

    std::cout << "num_states=" << num_states << ", alphabet_size=" << alphabet_size << "\n";
    std::cout << "initial_states=" << std::hex << initial_states;
    std::cout << ", final_states=" << std::hex << final_states << "\n";

    // ======================= WRITE TO OUTPUT FILE ========================= //

    std::cout << "Safra's algorithm done. Writing result to file " << argv[2];
    std::cout << "..." << std::endl;

    // Open output file
    outfile.open(argv[2], std::ios::out);
    if (!outfile.is_open()) {
        std::cout << "ERROR: Improper output filename." << std::endl;
        return 1;
    }

    WriteRabin();

    // Close output file
    outfile.close();

    return 0;
}