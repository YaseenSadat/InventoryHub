/**
 * inventory_structures.h
 *
 * Header file for managing global B-tree and HashMap structures.
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

 #ifndef INVENTORY_STRUCTURES_H
 #define INVENTORY_STRUCTURES_H
 
 // --- B-tree Functions ---
 
 /**
  * Initializes the global B-tree.
  *
  * - Creates a new B-tree root node if the tree is uninitialized.
  * - Must be called before inserting keys if `insert_into_btree()` has not been used.
  */
 void init_btree();
 
 /**
  * Inserts a key into the global B-tree.
  *
  * - If the B-tree is uninitialized, it will be automatically created.
  * - Calls `btreeInsert()` to insert the key into the tree.
  *
  * @param key The integer key to insert into the B-tree.
  */
 void insert_into_btree(int key);
 
 /**
  * Traverses the global B-tree and prints its keys in ascending order.
  *
  * - Calls `btreeTraverse()` to perform an in-order traversal.
  * - Outputs the keys to `stdout`, followed by a newline.
  */
 void traverse_btree();
 
 // --- Hash Map Functions ---
 
 /**
  * Initializes the global hash map.
  *
  * - Creates a new hash map if uninitialized.
  * - Uses `createHashMap()` with `INITIAL_BUCKET_COUNT` buckets.
  */
 void init_hashmap();
 
 /**
  * Inserts a record into the global hash map.
  *
  * - If the hash map is uninitialized, it will be automatically created.
  * - Calls `insertRecord()` to add the record to the hash map.
  *
  * @param id The unique identifier for the record.
  * @param data A pointer to the string data associated with the record.
  */
 void insert_into_hashmap(int id, const char *data);
 
 /**
  * Retrieves a record's data from the global hash map.
  *
  * - Searches for the record with the given ID.
  * - Returns a pointer to the stored string if found, or NULL otherwise.
  * - The returned pointer **should not** be freed by the caller.
  *
  * @param id The unique identifier of the record to retrieve.
  * @return A pointer to the string data if found, or NULL if not found.
  */
 const char* get_from_hashmap(int id);
 
 // --- Cleanup Function ---
 
 /**
  * Frees all allocated memory for the global B-tree and hash map.
  *
  * - Calls `freeBTree()` to deallocate all B-tree nodes.
  * - Calls `freeHashMap()` to free hash map entries and buckets.
  * - Resets global pointers to NULL to prevent use after free.
  */
 void free_structures();
 
 #endif // INVENTORY_STRUCTURES_H
 