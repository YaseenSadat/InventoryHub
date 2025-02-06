#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "btree.h"

BTreeNode* createBTreeNode(bool leaf) {
    BTreeNode *node = (BTreeNode*) malloc(sizeof(BTreeNode));
    node->leaf = leaf;
    node->n = 0;
    for (int i = 0; i < 2 * T; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// Utility function to split the child of a node.
void btreeSplitChild(BTreeNode *parent, int i, BTreeNode *child) {
    BTreeNode *newChild = createBTreeNode(child->leaf);
    newChild->n = T - 1;

    // Copy the last (T-1) keys of child to newChild.
    for (int j = 0; j < T - 1; j++)
        newChild->keys[j] = child->keys[j + T];

    // Copy the last T children if child is not leaf.
    if (!child->leaf) {
        for (int j = 0; j < T; j++)
            newChild->children[j] = child->children[j + T];
    }
    child->n = T - 1;

    // Create space for new child in parent.
    for (int j = parent->n; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[i + 1] = newChild;

    // Move keys in parent.
    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = child->keys[T - 1];
    parent->n += 1;
}

// Insert a key into a non-full node.
void btreeInsertNonFull(BTreeNode *node, int key) {
    int i = node->n - 1;
    if (node->leaf) {
        // Find location to insert.
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    } else {
        // Find the child to insert into.
        while (i >= 0 && key < node->keys[i])
            i--;
        i++;
        // If child is full, split it.
        if (node->children[i]->n == 2 * T - 1) {
            btreeSplitChild(node, i, node->children[i]);
            if (key > node->keys[i])
                i++;
        }
        btreeInsertNonFull(node->children[i], key);
    }
}

void btreeInsert(BTreeNode **rootRef, int key) {
    BTreeNode *root = *rootRef;
    if (root->n == 2 * T - 1) {
        // Tree is full, need to grow in height.
        BTreeNode *newRoot = createBTreeNode(false);
        newRoot->children[0] = root;
        btreeSplitChild(newRoot, 0, root);
        int i = 0;
        if (newRoot->keys[0] < key)
            i++;
        btreeInsertNonFull(newRoot->children[i], key);
        *rootRef = newRoot;
    } else {
        btreeInsertNonFull(root, key);
    }
}

// A simple traversal (inorder) of B-tree nodes.
void btreeTraverse(BTreeNode *root) {
    if (root != NULL) {
        int i;
        for (i = 0; i < root->n; i++) {
            if (!root->leaf)
                btreeTraverse(root->children[i]);
            printf("%d ", root->keys[i]);
        }
        if (!root->leaf)
            btreeTraverse(root->children[i]);
    }
}

BTreeNode* btreeSearch(BTreeNode *root, int key) {
    int i = 0;
    while (i < root->n && key > root->keys[i])
        i++;
    if (i < root->n && root->keys[i] == key)
        return root;
    if (root->leaf)
        return NULL;
    return btreeSearch(root->children[i], key);
}

void freeBTree(BTreeNode *root) {
    if (root == NULL)
        return;
    if (!root->leaf) {
        for (int i = 0; i <= root->n; i++) {
            freeBTree(root->children[i]);
        }
    }
    free(root);
}
