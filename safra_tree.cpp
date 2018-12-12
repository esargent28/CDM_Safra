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
#include <unordered_set>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <queue>

#include "safra_tree.h"

// Constants
int BITS_PER_ITEM = 64;
int64_t EMPTY_SET = 0;


// ========== Standard constructor, copy constructor, & destructor ========== //

/*
 * TODO: Change arguments so that constructor takes automaton as input
 * Standard constructor: creates a Safra tree based on the initial state set of
 *   the Buchi automaton
 */
SafraTree::SafraTree(int num_states, int alphabet_size,
    std::vector<std::vector<int64_t>> transition, int64_t initial_states,
    int64_t final_states) {

    num_states_ = num_states;
    transition_rule_ = transition;
    initial_states_ = initial_states;
    final_states_ = final_states;

    unused_labels_ = new std::priority_queue<int>();

    //initialize priority queue that contains every number from 1 to 2*n
    for (int i = 1; i <= 2*num_states_; i++) {
        unused_labels_->push(i);
    }

    // Create initial node setup
    if (Intersect(initial_states_, final_states_) == EMPTY_SET) { 
        // Empty intersection between I and F
        // => Initial tree is (1 : I)
        root_ = new SafraNode(initial_states_, false, this);
    }
    else if (Difference(initial_states_, final_states_) == EMPTY_SET) {
        // I is a subset of F
        // => Initial tree is (1 : I!)
        root_ = new SafraNode(initial_states_, true, this);
    }
    else {
        // Otherwise
        // => Initial tree is (1 : I, 2 : I n F!)
        root_ = new SafraNode(initial_states_, false, this);
        SafraNode *child = new SafraNode(
            Intersect(initial_states_, final_states), true, this);
        root_->AppendChild(child);
    }

}

void SafraTree::CopyChildren(SafraNode *node, SafraNode *other_node,
    std::unordered_set<int> &used_labels) {

    for (SafraNode *other_child : other_node->GetChildren()) {
        // Add label into our used set, append an identical child to our node
        used_labels.insert(other_child->GetLabel());
        SafraNode *child = new SafraNode(other_child);
        node->AppendChild(child);

        // Recursively copy all of the child's children
        CopyChildren(child, other_child, used_labels);
    }
}


/*
 * Transition constructor: Given an original SafraTree and a character, produces
 *   a new SafraTree that corresponds to the transition from the given tree
 *   along the specified character
 */
SafraTree::SafraTree(SafraTree *original, const int &character) {

    // Part 1: Copy tree structure over

    std::unordered_set<int> used_labels = {};

    // Copy over data about automaton
    initial_states_ = original->initial_states_;
    final_states_ = original->final_states_;
    int num_states_ = original->num_states_;
    transition_rule_ = original->transition_rule_;

    // Copy nodes over, keeping track of which labels have been used
    root_ = new SafraNode(original->root_);
    used_labels.insert(root_->GetLabel());

    // Build unused_labels_ priority queue (only contains things not in our
    //   used_labels set)
    unused_labels_ = new std::priority_queue<int>();

    for (int i = 1; i <= 2*num_states_; i++) {
        if (used_labels.find(i) == used_labels.end()) {
            unused_labels_->push(i);
        }
    }

    // Part 2: Run all 6 steps new tree
    UnmarkAllNodes();
    UpdateStateSets(character);
    AttachChildren();
    HorizontalMerge();
    KillEmptyNodes();
    VerticalMerge();
}

/*
 * Destructor: Frees up all resources used by this Safra tree
 */
SafraTree::~SafraTree() {

    // free root and all its children
    delete root_;

    // free priority queue
    delete unused_labels_;
}


// ============= Public methods for each step of the algorithm ============== //

/*
 * STEP 1: Unmarks all nodes in the Safra tree
 */
void SafraTree::UnmarkAllNodes() {
    GetRoot()->UnmarkAll();
}

/*
 * STEP 2: For every node in the tree, replaces the label set according to the
 *   automaton's transition rules
 */
void SafraTree::UpdateStateSets(const int &c) {
    GetRoot()->TransitionStates(c);
}

void SafraTree::SafraNode::CreateChild() {
    int64_t parent_states = GetStates();

    int64_t child_states = Union(parent_states, GetTree()->final_states_);

    if (child_states == EMPTY_SET)
        return;

    bool child_is_marked = true;

    SafraNode *child_node = new SafraNode(child_states, child_is_marked, GetTree());

    AppendChild(child_node);
}

void SafraTree::SafraNode::AttachChildrenToAllNodes() {

    for (SafraNode *child : GetChildren()) {
        child->CreateChild();
    }
    CreateChild();
}

/*
 * STEP 3: For every node v, if v's label set shares at least one state with
 *   the final states, create a new rightmost child u to v. Set the label set
 *   of u to the intersection between v's label set and the final states, and
 *   mark u.
 */
void SafraTree::AttachChildren() {
    GetRoot()->AttachChildrenToAllNodes();
}

void SafraTree::SafraNode::HorizontalMergeNodeLevel() {

    int64_t seen_states = EMPTY_SET;

    for (SafraNode *child : GetChildren()) {

        int64_t child_states = child->GetStates();
        int64_t new_child_states = Difference(child_states, seen_states);
        seen_states = Union(seen_states, child_states);

        child->SetStates(new_child_states);
    }
}

void SafraTree::SafraNode::HorizontalMergeAllNodes() {

    for (SafraNode *child : GetChildren()) {
        child->HorizontalMergeAllNodes();
    }

    HorizontalMergeNodeLevel();
}

/*
 * STEP 4: For all new nodes u, remove all states in u's label set that already
 *   appear in nodes to the left of u.
 */
void SafraTree::HorizontalMerge() {
    GetRoot()->HorizontalMergeAllNodes();
}


void SafraTree::SafraNode::KillEmptyNodesNodeLevel() {
    
    int i = 0;

    while (i < GetChildren().size()) {
        SafraNode *child = GetChildren()[i];

        if (child->GetStates() == EMPTY_SET) {
            EraseChild(i);
        } else {
            child->KillEmptyNodesNodeLevel();
            i++;
        }
    }

}

/*
 * STEP 5: Remove all nodes with empty label sets.
 */
void SafraTree::KillEmptyNodes() {
    GetRoot()->KillEmptyNodesNodeLevel();
}

void SafraTree::SafraNode::VerticalMergeNodeLevel() {

    int64_t all_children_states = EMPTY_SET;

    for (SafraNode *child : GetChildren()) {
        all_children_states = Union(all_children_states, child->GetStates());
    }

    int64_t this_node_states = GetStates();

    if (this_node_states == all_children_states) {

        // Mark parent, kill children        
        SetMarked(true);

        while (GetChildren().size() > 0) {
            EraseChild(0);
        }
    }

    else {
        for (SafraNode *child : GetChildren()) {
            child->VerticalMergeNodeLevel();
        }
    }
}

/*
 * STEP 6: Mark all states v such that v's label set is the union of all of its
 *   children's label sets
 */
void SafraTree::VerticalMerge() {
    GetRoot()->VerticalMergeNodeLevel();
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

int SafraTree::GetNewLabel() {
    int new_label = unused_labels_->top();
    unused_labels_->pop();

    return new_label;
}

void SafraTree::RemoveLabel(int label) {
    unused_labels_->push(label);
}

int64_t SafraTree::GetFinalStates() {
    return final_states_;
}

int64_t SafraTree::GetInitialStates() {
    return initial_states_;
}

SafraTree::SafraNode *SafraTree::GetRoot() {
    return root_;
}

// ========================================================================== //
// ======================= SAFRA NODE IMPLEMENTATION ======================== //
// ========================================================================== //


// =================== SafraNode Constructor & Destructor =================== //

SafraTree::SafraNode::SafraNode(const int64_t &states, const bool &marked, SafraTree *tree) {

    tree_ = tree;
    states_ = states;
    label_ = GetTree()->GetNewLabel();
    marked_ = marked;
}


SafraTree::SafraNode::SafraNode(SafraNode *other) {

    tree_ = other->tree_;
    states_ = other->states_;
    label_ = other->label_;
    marked_ = other->marked_;
}


SafraTree::SafraNode::~SafraNode() {

    GetTree()->RemoveLabel(GetLabel());
    for (SafraNode *child : GetChildren()) {
        delete child;
    }
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

    delete children_[i];
    children_.erase(children_.begin() + i);
}

SafraTree *SafraTree::SafraNode::GetTree() {
    return tree_;
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

// ================ String methods for SafraTree & SafraNode ================ //

/*
 * Writes out the string representation of a Safra node
 */
std::string SafraTree::SafraNode::ToString() {
    std::ostringstream stream;

    stream << GetLabel() << ":" << GetStates();
    if (IsMarked()) {
        stream << "!";
    }
    return stream.str();
}


/*
 * Recursive helper method for writing the string representation of a single
 *   Safra tree based on a given node's children
 */
std::string SafraTree::SafraNode::StringifyChildren() {

    std::ostringstream stream;

    for (SafraNode *child : GetChildren()) {
        stream << "," << child->ToString();
    }
    for (SafraNode *child : GetChildren()) {
        stream << child->StringifyChildren();
    }
    return stream.str();
}


/*
 * Writes out the string representation of a Safra tree
 */
std::string SafraTree::ToString() {

    std::ostringstream stream;
    stream << "(" << root_->ToString() << root_->StringifyChildren() << ")";
    return stream.str();
}