/* ************************************************************************** *
 *                                                                            *
 *                15-354: Computational Discrete Mathematics                  *
 *                     Final Project: Safra's Algorithm                       *
 *             Erik Sargent (esargent), Vaidehi Srinivas (vaidehis)           *
 *                                                                            *
 *        safra_tree.cpp - implementation of Safra tree data structure        *
 *                                                                            *
 * ************************************************************************** */

#include <vector>
#include <unordered_map>
#include <set>
#include <cassert>
#include <iostream>

#include "safra_tree.h"


// ========== Standard constructor, copy constructor, & destructor ========== //

/*
 * TODO: Change arguments so that constructor takes automaton as input
 * Standard constructor: creates a Safra tree based on the initial state set of
 *   the Buchi automaton
 */
SafraTree::SafraTree() {

}

/*
 * Copy constructor: Given a pointer to another Safra tree, creates a copy
 */
SafraTree::SafraTree(SafraTree *other) {

}

/*
 * Destructor: Frees up all resources used by this Safra tree
 */
SafraTree::~SafraTree() {

}


// ============= Public methods for each step of the algorithm ============== //

/*
 * STEP 1: Unmarks all nodes in the Safra tree
 */
void SafraTree::UnmarkAllNodes() {

}

/*
 * STEP 2: For every node in the tree, replaces the label set according to the
 *   automaton's transition rules
 */
void SafraTree::UpdateStateSets(const char &c) {

}

/*
 * STEP 3: For every node v, if v's label set shares at least one state with
 *   the final states, create a new rightmost child u to v. Set the label set
 *   of u to the intersection between v's label set and the final states, and
 *   mark u.
 */
void SafraTree::AttachChildren() {

}

/*
 * STEP 4: For all new nodes u, remove all states in u's label set that already
 *   appear in nodes to the left of u.
 */
void SafraTree::HorizontalMerge() {

}

/*
 * STEP 5: Remove all nodes with empty label sets.
 */
void SafraTree::KillEmptyNodes() {

}

/*
 * STEP 6: Mark all states v such that v's label set is the union of all of its
 *   children's label sets
 */
void SafraTree::VerticalMerge() {

}


// ========================= Private helper methods ========================= //

/*
 * Given a set of states and a highest state number, creates a label list
 *   corresponding to the state set
 * REQUIRES: max_state >= max(states)
 */
SafraTree::label_list_t SafraTree::MakeLabelList(std::set<int> states,
        const int &max_state) {

    label_list_t L;
    return L;
}


// ============ Set operations for our label list implementation ============ //


/*
 * Union of L1 and L2 is done via bitwise OR of the bitvectors
 */
SafraTree::label_list_t SafraTree::Union(label_list_t L1, label_list_t L2) {

    assert(L1.size() == L2.size());

    label_list_t L_union;

    for (size_t i = 0; i < L1.size(); i++) {
        int64_t x1 = L1[i];
        int64_t x2 = L2[i];

        L_union.push_back(x1 | x2);
    }

    return L_union;
}


/*
 * Complement is done with bitwise NOT of the bitvector
 */
SafraTree::label_list_t SafraTree::Complement(label_list_t L) {

    label_list_t L_complement;

    for (int64_t x : L) {
        L_complement.push_back(~x);
    }

    return L_complement;
}


/*
 * Intersection is done with bitwise AND of the bitvectors
 */
SafraTree::label_list_t SafraTree::Intersection(label_list_t L1, label_list_t L2) {

    assert(L1.size() == L2.size());

    label_list_t L_intersect;

    for (size_t i = 0; i < L1.size(); i++) {
        int64_t x1 = L1[i];
        int64_t x2 = L2[i];

        L_intersect.push_back(x1 & x2);
    }

    return L_intersect;
}


/*
 * Difference is done with bitwise AND and NOT of the bitvectors (L1 & (~L2))
 */
SafraTree::label_list_t SafraTree::Difference(label_list_t L1, label_list_t L2) {

    assert(L1.size() == L2.size());

    label_list_t L_diff;

    for (size_t i = 0; i < L1.size(); i++) {
        int64_t x1 = L1[i];
        int64_t x2 = L2[i];

        L_diff.push_back(x1 & (~x2));
    }

    return L_diff;
}


/*
 * Checking for inclusion is done by checking if the ith bit is 1
 */
bool SafraTree::Contains(label_list_t L, const int &i) {

    int outer_index = i / SafraTree::BITS_PER_ITEM;
    int inner_index = i % SafraTree::BITS_PER_ITEM;
    
    return (((L[outer_index] << inner_index) & 1) == 1);
}