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
    std::vector<int64_t> transition, int64_t initial_states,
    int64_t final_states) {

    num_states_ = num_states;
    transition_rule_ = std::vector<int64_t>(transition);
    initial_states_ = initial_states;
    final_states_ = final_states;

    unused_labels_ = new std::priority_queue<int, std::vector<int>, std::greater<int>>();

    //initialize priority queue that contains every number from 1 to 2*n
    for (int i = 0; i < 2*num_states_; i++) {
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
        SafraNode *child = new SafraNode(other_child, this);

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

    std::unordered_set<int> used_labels;

    // Copy over data about automaton
    initial_states_ = original->initial_states_;
    final_states_ = original->final_states_;
    num_states_ = original->num_states_;
    transition_rule_ = std::vector<int64_t>(original->transition_rule_);

    // Copy nodes over, keeping track of which labels have been used
    root_ = new SafraNode(original->root_, this);
    used_labels.insert(root_->GetLabel());
    CopyChildren(root_, original->GetRoot(), used_labels);

    // Build unused_labels_ priority queue (only contains things not in our
    //   used_labels set)
    unused_labels_ = new
        std::priority_queue<int, std::vector<int>,std::greater<int>>();

    for (int i = 0; i < 2*num_states_; i++) {
        if (used_labels.find(i) == used_labels.end()) {
            unused_labels_->push(i);
        }
    }

    // Part 2: Run all 6 steps on the new tree
    UnmarkAndUpdateAll(character);
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



// ========================================================================== //
// ============= Public methods for each step of the algorithm ============== //
// ========================================================================== //


/*
 * Step 1: Unmark all nodes in the tree
 * Step 2: Update all state sets in the tree according to the Buechi automaton's
 *   transition system
 */

void SafraTree::SafraNode::UnmarkAndUpdate(const int &c) {

    SetMarked(false);
    int64_t new_states = 0;
    for (int i = 0; i < tree_->num_states_; i++) {
        if (((states_ >> i) & 1) == 1) {
            new_states = tree_->Union(new_states, tree_->Transition(i, c));
        }
    }
    SetStates(new_states);

    for (SafraNode *child : children_) {
        child->UnmarkAndUpdate(c);
    }
}

void SafraTree::UnmarkAndUpdateAll(const int &c) {
    GetRoot()->UnmarkAndUpdate(c);
}


/*
 * STEP 3: For every node v, if v's label set shares at least one state with
 *   the final states, create a new rightmost child u to v. Set the label set
 *   of u to the intersection between v's label set and the final states, and
 *   mark u.
 */

void SafraTree::SafraNode::CreateChild() {

    int64_t parent_states = GetStates();
    int64_t child_states = Intersect(parent_states, GetTree()->final_states_);

    if (child_states != EMPTY_SET) {

        bool child_is_marked = true;
        SafraNode *child = new SafraNode(child_states, child_is_marked,
            GetTree());
        AppendChild(child);
    }
}

void SafraTree::SafraNode::AttachChildrenToAllNodes() {
    for (SafraNode *child : GetChildren()) {
        child->AttachChildrenToAllNodes();
    }
    CreateChild();
}


void SafraTree::AttachChildren() {
    GetRoot()->AttachChildrenToAllNodes();
}

/*
 * STEP 4: For all new nodes u, remove all states in u's label set (as well as
 *   the state sets of u's children) that already appear in u's older siblings.
 */
void SafraTree::SafraNode::RecursiveRemoveFromStates(int64_t r_states) {

    SetStates(Difference(GetStates(), r_states));

    for (SafraNode *child : GetChildren()) {
        child->RecursiveRemoveFromStates(r_states);
    }
}

void SafraTree::SafraNode::HorizontalMergeNodeLevel() {

    int64_t seen_states = EMPTY_SET;

    for (SafraNode *child : GetChildren()) {
        int64_t child_states = child->GetStates();
        child->RecursiveRemoveFromStates(seen_states);
        seen_states = Union(seen_states, child_states);
    }
}



void SafraTree::SafraNode::HorizontalMergeAllNodes() {

    for (SafraNode *child : GetChildren()) {
        child->HorizontalMergeAllNodes();
    }

    HorizontalMergeNodeLevel();
}


void SafraTree::HorizontalMerge() {
    GetRoot()->HorizontalMergeAllNodes();
}

/*
 * STEP 5: Remove all nodes with empty label sets.
 */
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


void SafraTree::KillEmptyNodes() {
    GetRoot()->KillEmptyNodesNodeLevel();
}


/*
 * STEP 6: Mark all states v such that v's label set is the union of all of its
 *   children's label sets
 */
void SafraTree::SafraNode::VerticalMergeNodeLevel() {

    int64_t all_children_states = EMPTY_SET;

    if (GetStates() == EMPTY_SET) {
        return;
    }

    for (SafraNode *child : GetChildren()) {
        all_children_states = Union(all_children_states, child->GetStates());
    }

    int64_t this_node_states = GetStates();

    if (this_node_states == all_children_states) {

        // Mark parent, kill children        
        SetMarked(true);

        for (SafraNode *child : children_) {
            delete child;
        }
        children_.clear();
    }
    else {
        for (SafraNode *child : GetChildren()) {
            child->VerticalMergeNodeLevel();
        }
    }
}


void SafraTree::VerticalMerge() {
    GetRoot()->VerticalMergeNodeLevel();
}


void SafraTree::SafraNode::GetLabelInfoNodeLevel(bool **seen_states) {
    // working with the assumption that labels can appear a maximum
    // of once in the tree
    int this_label = GetLabel();

    if (IsMarked()) {
        seen_states[this_label][1] = true;
    } else {
        seen_states[this_label][0] = true;
    }

    for (SafraNode *child : GetChildren()) {
        child->GetLabelInfoNodeLevel(seen_states);
    }
}

/* 
 * This is used to calculate the final accepting Rabin pairs.
 * This method takes in a tree and the number of possible labels
 * It then returns an array of two boolean arrays: B
 *      - B[i][0] is true if i is in the tree but not marked
 *      - B[i][1] is true if i is the the tree and is marked
 */
bool **SafraTree::GetLabelInfo(int num_labels) {
    SafraNode *root = GetRoot();

    bool **seen_states = new bool *[num_labels];

    for (int i = 0; i < num_labels; i++) {
        seen_states[i] = new bool[2];
        seen_states[i][0] = false;
        seen_states[i][1] = false;
    }
    root->GetLabelInfoNodeLevel(seen_states);

    return seen_states;
}


// ========================= Private helper methods ========================= //

int64_t SafraTree::Transition(const int &state, const int &character) {
    assert(character * num_states_ + state < transition_rule_.size());
    return transition_rule_[character * num_states_ + state];
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

SafraTree::SafraNode::SafraNode(const int64_t &states, const bool &marked,
    SafraTree *tree) {

    tree_ = tree;
    states_ = states;
    label_ = GetTree()->GetNewLabel();
    marked_ = marked;
}


SafraTree::SafraNode::SafraNode(SafraNode *other, SafraTree *my_tree) {

    tree_ = my_tree;
    states_ = other->states_;
    label_ = other->label_;
    marked_ = other->marked_;
}


SafraTree::SafraNode::~SafraNode() {

    GetTree()->RemoveLabel(GetLabel());
    for (SafraNode *child : children_) {
        delete child;
    }
    children_.clear();
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

std::vector<SafraTree::SafraNode *> &SafraTree::SafraNode::GetChildren() {
    return children_;
}

void SafraTree::SafraNode::AppendChild(SafraNode *child) {
    if (child == this) {
        // ERROR: Node cannot be its own child
        return;
    }
    children_.push_back(child);
}

void SafraTree::SafraNode::EraseChild(const int &i) {

    assert(i < children_.size());

    delete children_[i];
    children_.erase(children_.begin() + i);
}

SafraTree *SafraTree::SafraNode::GetTree() {
    return tree_;
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

    stream << GetLabel()+1 << ":{";
    int first = true;
    for (int i = 0; i < tree_->num_states_; i++) {
        if (((states_ >> i) & 1) == 1) {
            if (!first) { stream << ","; }
            else { first = false; }
            stream << i+1;
        }
    }
    stream << "}";
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
        stream << "; " << child->ToString();
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