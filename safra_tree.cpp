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
#include <cstdint>

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
    root_->UnmarkAll();
}

/*
 * STEP 2: For every node in the tree, replaces the label set according to the
 *   automaton's transition rules
 */
void SafraTree::UpdateStateSets(const int &c) {

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
 * Given a set of states (which is a subset of {0, 1, ..., 63}), creates a
 *   bitvector where state i is in the set if and only if the ith bit of the
 *   resulting bitvector is 1
 */
int64_t SafraTree::MakeBitvector(std::set<int> states) {

    int64_t bitvector = 0;

    for (int state : states) {
        bitvector |= (1 << state);
    }

    return bitvector;
}


int64_t SafraTree::Transition(const int &state, const int &character) {
    return transition_rule_[state][character];
}

// ========================================================================== //
// ======================= SAFRA NODE IMPLEMENTATION ======================== //
// ========================================================================== //


// =================== SafraNode Constructor & Destructor =================== //

SafraTree::SafraNode::SafraNode(const int64_t &states, const int &label,
    const bool &marked) {

    states_ = states;
    label_ = label;
    marked_ = marked;
}


SafraTree::SafraNode::~SafraNode() {

}


// ============== Access methods for SafraNode member variables ============= //

int64_t SafraTree::SafraNode::GetStates() {
    return states_;
}

void SafraTree::SafraNode::SetStates(const int64_t &states) {
    states_ = states;
}

int SafraTree::SafraNode::GetLabel() {
    return label_;
}

void SafraTree::SafraNode::SetLabel(const int &label) {
    label_ = label;
}

bool SafraTree::SafraNode::IsMarked() {
    return marked_;
}

void SafraTree::SafraNode::SetMarked(const bool &marked) {
    marked_ = marked;
}

std::vector<SafraTree::SafraNode *> SafraTree::SafraNode::GetChildren() {
    return children_;
}

void SafraTree::SafraNode::AppendChild(SafraNode *child) {
    children_.push_back(child);
}

void SafraTree::SafraNode::EraseChild(const int &i) {
    children_.erase(children_.begin() + i);
}


// ============== Node methods for steps of Safra's Algorithm =============== //

/*
 * Safra's Algorithm Step 1: Unmarking All Nodes
 *  For the current node N, unmarks N and all nodes in the subtree rooted at N
 */
void SafraTree::SafraNode::UnmarkAll() {

    SetMarked(false);
    for (SafraNode *child : children_) {
        child->UnmarkAll();
    }
}


/*
 * Safra's Algorithm Step 2: Transitioning State Sets
 *   For the current node N, replaces N's state set with its out-neighbors
 *   according to the transition rule.
 */
void SafraTree::SafraNode::TransitionStates(const int &c) {

    int64_t new_states = 0;

    for (int i = 0; i < tree_->num_states_; i++) {
        if ((states_ << i) & 1 == 1) {
            new_states = tree_->Union(new_states, tree_->Transition(i, c));
        }
    }

    SetStates(new_states);
    for (SafraNode *child : children_) {
        child->TransitionStates(c);
    }
}


// ============== Set operations for label list implementation ============== //

int64_t SafraTree::Union(const int64_t &x, const int64_t &y) {
    return x | y;
}

int64_t SafraTree::Intersect(const int64_t &x, const int64_t &y) {
    return x & y;
}

int64_t SafraTree::Complement(const int64_t &x) {
    return ~x;
}

int64_t SafraTree::Difference(const int64_t &x, const int64_t &y) {
    return x & (~y);
}

bool SafraTree::Contains(const int64_t &x, const int &i) {
    return ((x << i) & 1) == 1;
}

int64_t SafraTree::Insert(const int64_t &x, const int &i) {
    return x | (1 << i);
}

int64_t SafraTree::Remove(const int64_t &x, const int &i) {
    return x & (~(1 << i));
}