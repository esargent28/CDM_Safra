/* ************************************************************************** *
 *                                                                            *
 *                15-354: Computational Discrete Mathematics                  *
 *                     Final Project: Safra's Algorithm                       *
 *             Erik Sargent (esargent), Vaidehi Srinivas (vaidehis)           *
 *                                                                            *
 *            safra_tree.cpp - header for Safra tree data structure           *
 *                                                                            *
 * ************************************************************************** */

#include <vector>
#include <unordered_map>
#include <set>
#include <iostream>

class SafraTree {
public:

    // Standard constructor, copy constructor, & destructor
    SafraTree(); // TODO -- change arguments to make tree for initial states
    SafraTree(SafraTree *original);
    ~SafraTree();

    // Public methods for each step of the algorithm
    void UnmarkAllNodes();               // (1)
    void UpdateStateSets(const char &c); // (2)
    void AttachChildren();               // (3)
    void HorizontalMerge();              // (4)
    void KillEmptyNodes();               // (5)
    void VerticalMerge();                // (6)

private:
    typedef std::vector<int64_t> label_list_t;

    // Private helper functions
    label_list_t MakeLabelList(std::set<int> states, const int &max_state);

    // Set operations using our label list implementation
    label_list_t Union(label_list_t L1, label_list_t L2);
    label_list_t Complement(label_list_t L);
    label_list_t Intersection(label_list_t L1, label_list_t L2);
    label_list_t Difference(label_list_t L1, label_list_t L2);
    bool Contains(label_list_t L, const int &i);

    // Member variables representing Buchi automaton
    label_list_t initial_states;
    label_list_t final_states;
    int num_states;

    std::unordered_map<int, std::unordered_map<char, int>> transition_rule;
    // TODO: make data structure for single node, put root node ptr here


    static constexpr int BITS_PER_ITEM = 64;
};


