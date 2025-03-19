/**
 * btree.h
 *
 * Header file for the B-tree implementation.
 *
 * A B-tree is a self-balancing search tree that maintains sorted data 
 * and allows searches, sequential access, insertions, and deletions 
 * in logarithmic time.
 *
 * This file defines the structure of a B-tree node and declares 
 * the essential functions for B-tree operations.
 */

 #ifndef BTREE_H
 #define BTREE_H
 
 #include <stdbool.h>
 
 /**
  * The minimum degree (T) of the B-tree.
  * 
  * - Each node (except the root) must contain at least (T - 1) keys.
  * - Each node can contain at most (2 * T - 1) keys.
  * - Each internal node (except leaf) has between T and (2 * T) children.
  * - Adjust the value of T based on desired performance and space constraints.
  */
 #define T 3  // Minimum degree (T). Adjust as needed.
 
 /**
  * Structure defining a B-tree node.
  *
  * A B-tree node contains:
  * - `n`       : The current number of keys stored in the node.
  * - `keys[]`  : An array of keys (maximum of 2 * T - 1).
  * - `children[]` : An array of pointers to child nodes (maximum of 2 * T).
  * - `leaf`    : A boolean flag indicating whether the node is a leaf.
  */
 typedef struct BTreeNode {
     int n;                          // Current number of keys stored in the node
     int keys[2 * T - 1];             // Array storing keys (max 2 * T - 1 keys)
     struct BTreeNode *children[2 * T]; // Pointers to child nodes (max 2 * T children)
     bool leaf;                      // True if node is a leaf, otherwise false
 } BTreeNode;
 
 /**
  * Creates a new B-tree node.
  * 
  * @param leaf A boolean indicating whether the node is a leaf.
  * @return A pointer to the newly allocated BTreeNode.
  */
 BTreeNode* createBTreeNode(bool leaf);
 
 /**
  * Inserts a key into the B-tree.
  *
  * If the root node is full, the tree grows in height.
  * This function ensures that keys remain sorted and that the B-tree 
  * properties are maintained.
  *
  * @param rootRef A reference to the root node of the B-tree.
  * @param key The key to insert.
  */
 void btreeInsert(BTreeNode **root, int key);
 
 /**
  * Performs an in-order traversal of the B-tree.
  *
  * This function prints the keys in ascending order by visiting child nodes 
  * recursively.
  *
  * @param root The root node of the B-tree.
  */
 void btreeTraverse(BTreeNode *root);
 
 /**
  * Searches for a key in the B-tree.
  *
  * This function searches for the given key in the B-tree and returns a pointer 
  * to the node that contains the key. If the key is not found, it returns NULL.
  *
  * @param root The root node of the B-tree.
  * @param key The key to search for.
  * @return A pointer to the node containing the key, or NULL if not found.
  */
 BTreeNode* btreeSearch(BTreeNode *root, int key);
 
 /**
  * Frees the memory allocated for the entire B-tree.
  *
  * This function recursively deallocates memory for all nodes in the tree.
  *
  * @param root The root node of the B-tree.
  */
 void freeBTree(BTreeNode *root);
 
 #endif // BTREE_H
 