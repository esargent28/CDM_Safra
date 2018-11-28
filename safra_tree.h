/* ************************************************************************** *
 *                                                                            *
 *                15-354: Computational Discrete Mathematics                  *
 *                     Final Project: Safra's Algorithm                       *
 *             Erik Sargent (esargent), Vaidehi Srinivas (vaidehis)           *
 *                                                                            *
 *             safra_tree.h - header for Safra tree data structure            *
 *                                                                            *
 * ************************************************************************** */

#include <vector>
#include <unordered_map>
#include <set>
#include <iostream>
#include <cstdint>

#include "safra_node.h"

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
    // Member variables representing Buchi automaton
    int64_t initial_states_;
    int64_t final_states_;
    int num_states_;
    std::vector<std::unordered_map<char, std::set<int>>> transition_rule_;
    SafraNode *root_;

    // Private helper methods
    static int64_t MakeBitvector(std::set<int> states);

    // Implementation of set functions using bitvector implementation
    static int64_t Union(const int64_t &x, const int64_t &y);
    static int64_t Intersect(const int64_t &x, const int64_t &y);
    static int64_t Complement(const int64_t &x);
    static int64_t Difference(const int64_t &x, const int64_t &y);
    static bool Contains(const int64_t &x, const int &i);
    static int64_t Insert(const int64_t &x, const int &i);
    static int64_t Remove(const int64_t &x, const int &i);

    static constexpr int BITS_PER_ITEM = 64;
};


