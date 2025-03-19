/**
 * hashmap.h
 *
 * Header file for a simple hash map implementation using separate chaining.
 *
 * This module provides an efficient way to store and retrieve key-value pairs
 * using a hash table. It supports:
 * - Dynamic memory allocation for records.
 * - Collision resolution via linked lists (separate chaining).
 * - Fast lookup, insertion, and deletion operations.
 *
 * Usage:
 * - Call `createHashMap()` to initialize the hash table.
 * - Use `insertRecord()` to add key-value pairs.
 * - Retrieve data using `getRecord()`.
 * - Delete entries with `deleteRecord()`.
 * - Free all allocated memory using `freeHashMap()`.
 */

 #ifndef HASHMAP_H
 #define HASHMAP_H
 
 #include <stdbool.h>  // Provides `bool` type for function return values
 
 /**
  * The initial number of buckets in the hash map.
  *
  * - A prime number is chosen (101) to reduce hash collisions.
  * - Can be adjusted based on the expected number of elements.
  */
 #define INITIAL_BUCKET_COUNT 101
 
 /**
  * Structure representing a record stored in the hash map.
  *
  * Fields:
  * - `id`   : A unique integer identifier (key).
  * - `data` : A dynamically allocated string containing the record data.
  */
 typedef struct Record {
     int id;          ///< Unique identifier for the record.
     char *data;      ///< Pointer to dynamically allocated string data.
 } Record;
 
 /**
  * Structure representing a node in the hash map's linked list.
  *
  * Each bucket in the hash table contains a linked list of `HashNode` elements.
  * This handles hash collisions by chaining multiple records within the same bucket.
  *
  * Fields:
  * - `record` : The actual data stored in the node.
  * - `next`   : Pointer to the next node in the list (NULL if last).
  */
 typedef struct HashNode {
     Record record;        ///< The record stored in this node.
     struct HashNode *next; ///< Pointer to the next node in the linked list.
 } HashNode;
 
 /**
  * Structure representing the hash map.
  *
  * Fields:
  * - `bucketCount` : The total number of buckets in the hash table.
  * - `buckets`     : An array of pointers to `HashNode` linked lists.
  */
 typedef struct HashMap {
     int bucketCount;      ///< The number of buckets in the hash map.
     HashNode **buckets;   ///< Array of pointers to linked list buckets.
 } HashMap;
 
 /**
  * Creates a new hash map with the specified number of buckets.
  *
  * @param bucketCount The number of buckets for the hash table.
  * @return A pointer to the newly allocated HashMap structure, or NULL on failure.
  */
 HashMap* createHashMap(int bucketCount);
 
 /**
  * Frees the memory allocated for the entire hash map.
  *
  * This function deallocates all nodes and records stored in the hash table.
  *
  * @param map A pointer to the HashMap to be freed.
  */
 void freeHashMap(HashMap *map);
 
 /**
  * Inserts a record into the hash map.
  *
  * Uses separate chaining to resolve collisions.
  *
  * @param map A pointer to the HashMap.
  * @param rec The record to be inserted.
  * @return True if the insertion is successful, false otherwise.
  */
 bool insertRecord(HashMap *map, Record rec);
 
 /**
  * Retrieves a record from the hash map based on the given ID.
  *
  * Searches for the record in the appropriate bucket and returns it.
  *
  * @param map A pointer to the HashMap.
  * @param id The ID of the record to retrieve.
  * @return A pointer to the found Record, or NULL if not found.
  */
 Record* getRecord(HashMap *map, int id);
 
 /**
  * Deletes a record from the hash map.
  *
  * If the record is found, it is removed from the linked list and memory is freed.
  *
  * @param map A pointer to the HashMap.
  * @param id The ID of the record to delete.
  * @return True if the record was successfully deleted, false if not found.
  */
 bool deleteRecord(HashMap *map, int id);
 
 /**
  * Computes a hash index for a given key using the modulus operation.
  *
  * The function returns an index within the valid range of buckets.
  *
  * @param key The integer key to be hashed.
  * @param bucketCount The total number of buckets in the hash map.
  * @return The computed bucket index.
  */
 unsigned int hashFunction(int key, int bucketCount);
 
 #endif // HASHMAP_H
 