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
#include <string>
#include <cstdint>

class SafraTree {
public:

    // Standard constructor, copy constructor, & destructor
    SafraTree(); // TODO -- change arguments to make tree for initial states
    SafraTree(SafraTree *original);
    ~SafraTree();

    // Public methods for each step of the algorithm
    void UnmarkAllNodes();               // (1)
    void UpdateStateSets(const int &c); // (2)
    void AttachChildren();               // (3)
    void HorizontalMerge();              // (4)
    void KillEmptyNodes();               // (5)
    void VerticalMerge();                // (6)

private:

    // ===== Safra node class definition =====

    class SafraNode {
    public:

        // Constructor & Destructor
        SafraNode(const int64_t &states, const int &label, const bool &marked);
        ~SafraNode();

        // Access methods for member variables
        int64_t GetStates();
        void SetStates(const int64_t &states);

        int GetLabel();
        void SetLabel(const int &label);

        bool IsMarked();
        void SetMarked(const bool &marked);

        std::vector<SafraNode *> GetChildren();
        void AppendChild(SafraNode *child);
        void EraseChild(const int &i);


        // Methods for steps of Safra's Algorithm
        void UnmarkAll();
        void TransitionStates(const int &c);

    private:
        // Member variables
        int64_t states_;
        int label_;
        bool marked_;
        std::vector<SafraNode *> children_;
        SafraTree *tree_;
    };

    // Member variables representing Buchi automaton
    int64_t initial_states_;
    int64_t final_states_;
    int num_states_;
    std::vector<std::vector<int64_t>> transition_rule_;

    SafraNode *root_;

    // Private helper methods
    int64_t Transition(const int &state, const int &character);

    // Static helper methods
    int64_t MakeBitvector(std::set<int> states);

    // Implementation of set functions using bitvector implementation
    int64_t Union(const int64_t &x, const int64_t &y);
    int64_t Intersect(const int64_t &x, const int64_t &y);
    int64_t Complement(const int64_t &x);
    int64_t Difference(const int64_t &x, const int64_t &y);
    bool Contains(const int64_t &x, const int &i);
    int64_t Insert(const int64_t &x, const int &i);
    int64_t Remove(const int64_t &x, const int &i);

    static constexpr int BITS_PER_ITEM = 64;
};


