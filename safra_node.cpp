/* ************************************************************************** *
 *                                                                            *
 *                15-354: Computational Discrete Mathematics                  *
 *                     Final Project: Safra's Algorithm                       *
 *             Erik Sargent (esargent), Vaidehi Srinivas (vaidehis)           *
 *                                                                            *
 *            safra_node.h - header file for individual tree nodes            *
 *                                                                            *
 * ************************************************************************** */

#include <vector>
#include <cstdint>

#include "safra_node.h"


// ======================== Constructor & Destructor ======================== //

SafraNode::SafraNode(const int64_t &states, const int &label, const bool &marked) {
    states_ = states;
    label_ = label;
    marked_ = marked;
}


SafraNode::~SafraNode() {

}


// =================== Access methods for member variables ================== //

int64_t SafraNode::GetStates() {
    return states_;
}

void SafraNode::SetStates(const int64_t &states) {
    states_ = states;
}

int SafraNode::GetLabel() {
    return label_;
}

void SafraNode::SetLabel(const int &label) {
    label_ = label;
}

bool SafraNode::IsMarked() {
    return marked_;
}

void SafraNode::SetMarked(const bool &marked) {
    marked_ = marked;
}

std::vector<SafraNode *> SafraNode::GetChildren() {
    return children_;
}

void SafraNode::AppendNewChild(SafraNode *child) {
    children_.push_back(child);
}

void SafraNode::EraseChild(const int &i) {
    children_.erase(children_.begin() + i);
}


// ================= Methods for steps of Safra's Algorithm ================= //

/*
 * Safra's Algorithm Step 1: Unmarking All Nodes
 *  For the current node N, unmarks N and all nodes in the subtree rooted at N
 */
void SafraNode::UnmarkAll() {

    SetMarked(false);
    for (SafraNode *child : children_) {
        child->UnmarkAll();
    }
}


