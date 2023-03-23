#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;

int heightHelper(Node* root, int currentLevel, int* leafHeight)
{
    // 1 = true, 0 = false
    // Base Case: tree is empty, therefore implicitly true
    if (root == nullptr)
    {
        return 1;
    }

    // If we are at a leaf node, compare heights
    if (root -> right == nullptr && root -> left == nullptr)
    {

        // Check if this is the first leaf node we have encountered
        if (*leafHeight == 0)
        {

            // Then it is implicitly true that the levels are equivalent
            *leafHeight = currentLevel;
            return 1;
        }

        // If it is not the first leaf, then compare heights
        else
        {

            // If the previous leaf's height/level is different than the
            // current leaf's height/level, then return false
            if (*leafHeight != currentLevel)
            {
                return 0;
            }

            // If the previous leaf's height/level is the same as the
            // current leaf's height/level, then return true
            return 1;
        }
    }

    // Pre-order recursive call to do work and then visit subtrees
    return heightHelper(root -> left, currentLevel + 1, leafHeight) 
            && heightHelper(root -> right, currentLevel + 1, leafHeight);

}




bool equalPaths(Node * root)
{
    // Add your code below
    int currentLevel = 0;
    int leafHeight = 0;
    if (heightHelper(root, currentLevel, &leafHeight))
    {
        return true;
    }

    return false;

}