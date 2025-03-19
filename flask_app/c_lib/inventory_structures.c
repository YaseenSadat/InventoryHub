/**
 * data_structures.c
 *
 * Wrapper functions for managing global B-tree and HashMap data structures.
 *
 * This module provides:
 * - A global B-tree for efficient key-based insertions and traversals.
 * - A global hash map for fast key-value lookups and insertions.
 * - Initialization and cleanup functions to manage memory.
 *
 * Usage:
 * - Call `init_btree()` before inserting keys into the B-tree.
 * - Use `insert_into_btree()` to add keys.
 * - Call `traverse_btree()` to print stored keys.
 * - Call `init_hashmap()` before inserting records into the hash map.
 * - Use `insert_into_hashmap()` to add records.
 * - Retrieve records using `get_from_hashmap()`.
 * - Call `free_structures()` before program termination to release memory.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <string.h>
 #include "btree.h"
 #include "hashmap.h"
 
 // --- B-Tree Wrappers ---
 
 /**
  * Global pointer to the B-tree root node.
  * 
  * - The B-tree is initialized lazily when the first key is inserted.
  * - Uses `createBTreeNode()` to allocate memory for the root node.
  */
 static BTreeNode *globalBTree = NULL;
 
 /**
  * Initializes the global B-tree.
  *
  * - If the B-tree is already initialized, the function does nothing.
  * - If uninitialized, it creates a new B-tree with an empty root node.
  */
 void init_btree() {
     if (!globalBTree) {
         globalBTree = createBTreeNode(true);  // Create a leaf node as the root
     }
 }
 
 /**
  * Inserts a key into the global B-tree.
  *
  * - Initializes the B-tree if it is uninitialized.
  * - Calls `btreeInsert()` to insert the key into the tree.
  *
  * @param key The integer key to insert into the B-tree.
  */
 void insert_into_btree(int key) {
     if (!globalBTree) {
         init_btree();
     }
     btreeInsert(&globalBTree, key);
 }
 
 /**
  * Traverses the global B-tree and prints its keys in ascending order.
  *
  * - Calls `btreeTraverse()` to perform an in-order traversal.
  * - Outputs the keys to `stdout` followed by a newline.
  */
 void traverse_btree() {
     if (globalBTree) {
         btreeTraverse(globalBTree);
         printf("\n");
     }
 }
 
 // --- Hash Map Wrappers ---
 
 /**
  * Global pointer to the hash map.
  * 
  * - The hash map is initialized lazily when the first record is inserted.
  * - Uses `createHashMap()` to allocate memory for the hash table.
  */
 static HashMap *globalHashMap = NULL;
 
 /**
  * Initializes the global hash map.
  *
  * - If the hash map is already initialized, the function does nothing.
  * - If uninitialized, it creates a new hash map with `INITIAL_BUCKET_COUNT` buckets.
  */
 void init_hashmap() {
     if (!globalHashMap) {
         globalHashMap = createHashMap(INITIAL_BUCKET_COUNT);
     }
 }
 
 /**
  * Inserts a record into the global hash map.
  *
  * - Initializes the hash map if it is uninitialized.
  * - Allocates memory for the record's data using `strdup()`.
  * - Calls `insertRecord()` to add the record to the hash map.
  * - Frees the temporary copy of the record's data after insertion.
  *
  * @param id The unique identifier for the record.
  * @param data A pointer to the string data associated with the record.
  */
 void insert_into_hashmap(int id, const char *data) {
     if (!globalHashMap) {
         init_hashmap();
     }
     Record rec;
     rec.id = id;
     rec.data = strdup(data);  // Allocate memory and copy the string
     insertRecord(globalHashMap, rec);
     free(rec.data);  // Free temporary copy after insertion (insertRecord duplicates it)
 }
 
 /**
  * Retrieves a record's data from the global hash map.
  *
  * - Searches the hash map for a record with the given ID.
  * - Returns a pointer to the stored string if found, or NULL otherwise.
  * - The returned pointer should **not** be freed by the caller.
  *
  * @param id The unique identifier of the record to retrieve.
  * @return A pointer to the string data if found, or NULL if not found.
  */
 const char* get_from_hashmap(int id) {
     if (!globalHashMap) return NULL;
     Record *rec = getRecord(globalHashMap, id);
     return rec ? rec->data : NULL;
 }
 
 /**
  * Frees all allocated memory for the global B-tree and hash map.
  *
  * - Calls `freeBTree()` to recursively deallocate B-tree nodes.
  * - Calls `freeHashMap()` to deallocate hash map entries and buckets.
  * - Resets global pointers to NULL to prevent use after free.
  */
 void free_structures() {
     if (globalBTree) {
         freeBTree(globalBTree);
         globalBTree = NULL;
     }
     if (globalHashMap) {
         freeHashMap(globalHashMap);
         globalHashMap = NULL;
     }
 }
 