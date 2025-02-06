#ifndef BTREE_H
#define BTREE_H

#include <stdbool.h>

#define T 3  // Minimum degree (t). Adjust as needed.

// Define the B-tree node structure.
typedef struct BTreeNode {
    int n;                      // Current number of keys
    int keys[2 * T - 1];        // Array of keys
    struct BTreeNode *children[2 * T]; // Array of child pointers
    bool leaf;                // Is true when node is leaf. Otherwise false
} BTreeNode;

// Function declarations:
BTreeNode* createBTreeNode(bool leaf);
void btreeInsert(BTreeNode **root, int key);
void btreeTraverse(BTreeNode *root);
BTreeNode* btreeSearch(BTreeNode *root, int key);
void freeBTree(BTreeNode *root);

#endif // BTREE_H
