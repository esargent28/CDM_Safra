/* ************************************************************************** *
 *                                                                            *
 *                15-354: Computational Discrete Mathematics                  *
 *                     Final Project: Safra's Algorithm                       *
 *             Erik Sargent (esargent), Vaidehi Srinivas (vaidehis)           *
 *                                                                            *
 *          README.txt - general info on implementation & how to use          *
 *                                                                            *
 * ************************************************************************** */

// ========================================================================== //
// ================================== USAGE ================================= //
// ========================================================================== //

To run our implementation on a single input file / machine:
 - First, if the code has not already been compiled, run 'make' to compile the
    command-line application.
 - Then, run './safra <inputfilename> <outputfilename>' to read the Buechi
    automaton from <inputfilename>, compute the corresponding Rabin automaton,
	and write said Rabin automaton to <outputfilename>. Look to the next
    section for more information about the file formats.
 
A script has been included to run our Safra implementation on all of the test
machines provided. To run all tests, run './run_tests.sh'.
 
// ========================================================================== //
// =========================== INPUT FILE FORMAT ============================ //
// ========================================================================== //

  The input file read by the command-line application should specify the
    following characteristics of the Buechi automaton:
	 1) Number of states
	 2) Alphabet size
	 3) Number of transitions
	 4) All transitions
	 5) Initial states
	 6) Final states

All numbering should be 1-indexed rather than 0-indexed.

Between the dashed lines is an example to demonstrate the desired format:

------------
BUECHI
# Number of states
2
# Alphabet size
2
# Number of transitions
4
# begin transitions
1  1  1
2  1  2
1  2  1
1  2  2
# end transitions
# Buechi initial
1 
# Buechi final
2 
# Buechi eof
------------

If the input file does not match the deisired format, the command-line
  application will not accept it.

// ========================================================================== //
// ========================== OUTPUT FILE FORMAT ============================ //
// ========================================================================== //

  The output file written by the command-line application will specify the
    following characteristics of the corresponding Rabin automaton:
     1) Corresponding Buechi file name
	 2) Number of states
	 3) Alphabet size
	 4) Number of transitions
	 5) All transitions
	 6) Initial state
	 7) Rabin pairs
     8) Safra trees corresponding to each state

Between the dashed lines is an example to demonstrate the format:

------------
RABIN
# Buechi filename
test/buechi2.aut
# Number of states
3
# Alphabet size
2
# Number of transitions
6
# begin transitions
3  1  3
1  1  1
2  1  2
3  2  2
1  2  2
2  2  3
# end transitions
# Rabin initial
1
# begin Rabin pairs
L={ 1 3 }, R={ 2 }
L={ 2 1 }, R={ 3 }
# end Rabin pairs
# begin Safra trees
1: (1:{1})
2: (1:{1,2}; 2:{2}!)
3: (1:{1,2}; 3:{2}!)
# end Safra trees
# Rabin eof
------------


// ========================================================================== //
// ======================= OPTIMIZATIONS IMPLEMENTED ======================== //
// ========================================================================== //

For our implementation of the algorithm, we used multiple optimizations to
  ensure our program ran sufficiently quickly:

    1) We used a bitvector (int64_t) to represent a Safra node's set of states
        rather than a C++ STL container like std::set, std::vector, or
        std::unordered_set. While this added the restriction that the Buechi
        machines fed as input need to have 64 states or fewer, this greatly
        increased memory efficiency and runtime when computing transitions
        between Safra trees, especially since set operations such as union
        and intersection are reduced to bitwise OR and AND, respectively. Plus,
        most comprehensible Buechi automata are going to contain fewer than 64
        states.

    2) When computing transitions between trees, we did multiple steps of the
        algorithm in a single pass through all the nodes. For example, while
        algorithm as described in the notes first goes through and unmarks all
        nodes, then goes through again and updates their state sets, we did
        both operations in a single traversal of the tree, since the operations
        used are independent of each other

    3) We used a well-defined ToString method to convert the SafraTree pointer
        structure into an easily-comparable string to check for equality 
        between trees. Furthermore, we kept a hash map (std::unordered_map)
        that mapped the string representation back to the SafraTree object to
        allow for easy access to the tree structure without having to
        reconstruct it from scratch. Furthermore, we also use this mapping to
        establish the naming convention for the Rabin automaton's states, since
        the int state label for the tree is stored with the SafraTree object.



