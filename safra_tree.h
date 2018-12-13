/* ************************************************************************** *
 *                                                                            *
 *                15-354: Computational Discrete Mathematics                  *
 *                     Final Project: Safra's Algorithm                       *
 *             Erik Sargent (esargent), Vaidehi Srinivas (vaidehis)           *
 *                                                                            *
 *             safra_tree.h - header for Safra tree data structure            *
 *                                                                            *
 * ************************************************************************** */

#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <iostream>
#include <string>
#include <cstdint>
#include <queue>
#include <string>

class SafraTree {
public:

    // Standard constructor, copy constructor, & destructor
    SafraTree(int num_states, int alphabet_size,
        std::vector<int64_t> transition, int64_t initial_states,
        int64_t final_states);
    SafraTree(SafraTree *original, const int &character);
    ~SafraTree();

    // Public methods for each step of the algorithm
    void UnmarkAllNodes();              // (1)
    void UpdateStateSets(const int &c); // (2)
    void AttachChildren();              // (3)
    void HorizontalMerge();             // (4)
    void KillEmptyNodes();              // (5)
    void VerticalMerge();               // (6)

    // For getting Rabin Pairs
    bool** GetLabelInfo(int num_labels);

    // ToString method
    std::string ToString();

private:

    // ===== Safra node class definition =====

    class SafraNode {
    public:

        // Constructor & Destructor
        SafraNode(const int64_t &states, const bool &marked, SafraTree *tree);
        SafraNode(SafraNode *other, SafraTree *my_tree);
        ~SafraNode();

        // Access methods for member variables
        int64_t GetStates();
        void SetStates(const int64_t &states);

        int GetLabel();
        void SetLabel(const int &label);

        bool IsMarked();
        void SetMarked(const bool &marked);

        std::vector<SafraNode *> &GetChildren();
        void AppendChild(SafraNode *child);
        void EraseChild(const int &i);

        SafraTree *GetTree();

        std::string ToString();
        std::string StringifyChildren();

        // Methods for steps of Safra's Algorithm
        void UnmarkAll();
        void TransitionStates(const int &c);
        void CreateChild();
        void AttachChildrenToAllNodes();
        void HorizontalMergeNodeLevel();
        void HorizontalMergeAllNodes();
        void KillEmptyNodesNodeLevel();
        void VerticalMergeNodeLevel();

        // For getting RabinPairs
        void GetLabelInfoNodeLevel(bool **state_info);

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
    std::vector<int64_t> transition_rule_;

    SafraNode *root_;
    std::priority_queue<int, std::vector<int>, std::greater<int>> *unused_labels_;

    // root access method
    SafraNode *GetRoot();

    // Private helper methods
    int64_t Transition(const int &state, const int &character);
    int GetNewLabel();
    void RemoveLabel(int label);
    int64_t GetInitialStates();
    int64_t GetFinalStates();
    void CopyChildren(SafraNode *node, SafraNode *other_node,
        std::unordered_set<int> &used_labels);

    // Implementation of set functions using bitvector implementation
    static int64_t Union(const int64_t &x, const int64_t &y);
    static int64_t Intersect(const int64_t &x, const int64_t &y);
    static int64_t Complement(const int64_t &x);
    static int64_t Difference(const int64_t &x, const int64_t &y);
    static bool Contains(const int64_t &x, const int &i);
    static int64_t Insert(const int64_t &x, const int &i);
    static int64_t Remove(const int64_t &x, const int &i);
};


