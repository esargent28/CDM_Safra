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
    void AppendNewChild(SafraNode *child);
    void EraseChild(const int &i);


    // Methods for steps of Safra's Algorithm
    void UnmarkAll();

private:
    // Member variables
    int64_t states_;
    int label_;
    bool marked_;
    std::vector<SafraNode *> children_;
};
