/**
 * btree.c
 *
 * Implementation of a B-tree data structure.
 * 
 * A B-tree is a balanced tree structure optimized for read and write operations 
 * in databases and file systems. It allows efficient searching, insertion, 
 * and deletion while maintaining a balanced hierarchy.
 *
 * This file contains functions for:
 * - Creating a B-tree node
 * - Splitting child nodes when full
 * - Inserting keys into the tree
 * - Traversing and searching the tree
 * - Freeing allocated memory
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include "btree.h"
 
 /**
  * Creates a new B-tree node.
  *
  * @param leaf A boolean indicating whether the node is a leaf.
  * @return A pointer to the newly allocated B-tree node.
  */
 BTreeNode* createBTreeNode(bool leaf) {
     BTreeNode *node = (BTreeNode*) malloc(sizeof(BTreeNode));
     node->leaf = leaf;
     node->n = 0;
 
     // Initialize child pointers to NULL
     for (int i = 0; i < 2 * T; i++) {
         node->children[i] = NULL;
     }
     return node;
 }
 
 /**
  * Splits a full child node of a given parent node.
  *
  * @param parent The parent node.
  * @param i The index of the child in the parent's children array.
  * @param child The child node that needs to be split.
  */
 void btreeSplitChild(BTreeNode *parent, int i, BTreeNode *child) {
     BTreeNode *newChild = createBTreeNode(child->leaf);
     newChild->n = T - 1;
 
     // Copy the last (T-1) keys from child to newChild
     for (int j = 0; j < T - 1; j++)
         newChild->keys[j] = child->keys[j + T];
 
     // Copy child pointers if the child is not a leaf
     if (!child->leaf) {
         for (int j = 0; j < T; j++)
             newChild->children[j] = child->children[j + T];
     }
 
     child->n = T - 1; // Reduce the number of keys in child
 
     // Move existing children in parent to make space for newChild
     for (int j = parent->n; j >= i + 1; j--)
         parent->children[j + 1] = parent->children[j];
 
     // Link newChild to parent
     parent->children[i + 1] = newChild;
 
     // Move parent's keys to create space for the median key from child
     for (int j = parent->n - 1; j >= i; j--)
         parent->keys[j + 1] = parent->keys[j];
 
     // Move the median key from child to parent
     parent->keys[i] = child->keys[T - 1];
 
     // Increment parent's key count
     parent->n += 1;
 }
 
 /**
  * Inserts a key into a non-full B-tree node.
  *
  * @param node The node where the key should be inserted.
  * @param key The key to insert.
  */
 void btreeInsertNonFull(BTreeNode *node, int key) {
     int i = node->n - 1;
 
     // If the node is a leaf, find the correct position and insert the key
     if (node->leaf) {
         while (i >= 0 && key < node->keys[i]) {
             node->keys[i + 1] = node->keys[i];
             i--;
         }
         node->keys[i + 1] = key;
         node->n++;
     } else {
         // Find the child to insert the key into
         while (i >= 0 && key < node->keys[i])
             i--;
         i++;
 
         // If the chosen child is full, split it
         if (node->children[i]->n == 2 * T - 1) {
             btreeSplitChild(node, i, node->children[i]);
             
             // Determine which half to insert into
             if (key > node->keys[i])
                 i++;
         }
         btreeInsertNonFull(node->children[i], key);
     }
 }
 
 /**
  * Inserts a key into the B-tree.
  *
  * @param rootRef A pointer to the root node of the B-tree.
  * @param key The key to insert.
  */
 void btreeInsert(BTreeNode **rootRef, int key) {
     BTreeNode *root = *rootRef;
 
     // If the root is full, create a new root and increase tree height
     if (root->n == 2 * T - 1) {
         BTreeNode *newRoot = createBTreeNode(false);
         newRoot->children[0] = root;
 
         // Split the full root
         btreeSplitChild(newRoot, 0, root);
 
         // Decide which child to insert the key into
         int i = (newRoot->keys[0] < key) ? 1 : 0;
         btreeInsertNonFull(newRoot->children[i], key);
 
         *rootRef = newRoot; // Update the root reference
     } else {
         btreeInsertNonFull(root, key);
     }
 }
 
 /**
  * Traverses the B-tree in-order and prints keys.
  *
  * @param root The root node of the B-tree.
  */
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
 
 /**
  * Searches for a key in the B-tree.
  *
  * @param root The root node of the B-tree.
  * @param key The key to search for.
  * @return A pointer to the node containing the key, or NULL if not found.
  */
 BTreeNode* btreeSearch(BTreeNode *root, int key) {
     int i = 0;
 
     // Find the first key greater than or equal to the target key
     while (i < root->n && key > root->keys[i])
         i++;
 
     // If the key is found, return the node
     if (i < root->n && root->keys[i] == key)
         return root;
 
     // If reached a leaf node, key is not in the tree
     if (root->leaf)
         return NULL;
 
     // Search recursively in the appropriate child
     return btreeSearch(root->children[i], key);
 }
 
 /**
  * Recursively frees memory allocated for the B-tree.
  *
  * @param root The root node of the B-tree.
  */
 void freeBTree(BTreeNode *root) {
     if (root == NULL)
         return;
 
     // Recursively free child nodes if not a leaf
     if (!root->leaf) {
         for (int i = 0; i <= root->n; i++) {
             freeBTree(root->children[i]);
         }
     }
 
     // Free the current node
     free(root);
 }
 