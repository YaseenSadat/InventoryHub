/**
 * hashmap.c
 *
 * Implementation of a simple hash map using separate chaining for collision handling.
 *
 * This module provides efficient key-value storage and retrieval operations.
 * It is used to cache records in memory for fast access, reducing the need
 * for frequent database queries.
 *
 * Features:
 * - Uses a hash function to determine bucket indices.
 * - Supports insertion, retrieval, and deletion of records.
 * - Uses linked lists (separate chaining) to handle collisions.
 * - Ensures memory management by freeing allocated data when deleting records.
 */

 #include <stdlib.h>
 #include <string.h>
 #include "hashmap.h"
 
 /**
  * Computes a hash index for a given key using the modulus operation.
  *
  * @param key The integer key to be hashed.
  * @param bucketCount The total number of buckets in the hash map.
  * @return The computed bucket index.
  */
 unsigned int hashFunction(int key, int bucketCount) {
     return (unsigned int) key % bucketCount;
 }
 
 /**
  * Creates a new hash map with the specified number of buckets.
  *
  * @param bucketCount The number of buckets for storing records.
  * @return A pointer to the newly allocated HashMap structure, or NULL if allocation fails.
  */
 HashMap* createHashMap(int bucketCount) {
     HashMap *map = (HashMap*) malloc(sizeof(HashMap));
     if (!map) return NULL; // Allocation failed
 
     map->bucketCount = bucketCount;
     map->buckets = (HashNode**) calloc(bucketCount, sizeof(HashNode*));
     if (!map->buckets) {
         free(map); // Cleanup in case of failure
         return NULL;
     }
     return map;
 }
 
 /**
  * Frees the memory allocated for the hash map.
  *
  * Iterates through all buckets, deallocating nodes and stored records.
  *
  * @param map A pointer to the HashMap to be freed.
  */
 void freeHashMap(HashMap *map) {
     if (!map) return;
 
     // Iterate through each bucket and free linked list nodes
     for (int i = 0; i < map->bucketCount; i++) {
         HashNode *node = map->buckets[i];
         while (node) {
             HashNode *temp = node;
             node = node->next;
             free(temp->record.data);  // Free dynamically allocated record data
             free(temp);  // Free the node itself
         }
     }
 
     free(map->buckets);  // Free the array of bucket pointers
     free(map);  // Free the HashMap structure
 }
 
 /**
  * Inserts a record into the hash map.
  *
  * If a record with the same ID already exists in the bucket, it is stored as
  * a new node in the linked list (separate chaining).
  *
  * @param map A pointer to the HashMap.
  * @param rec The record to be inserted.
  * @return True if the insertion is successful, false otherwise.
  */
 bool insertRecord(HashMap *map, Record rec) {
     if (!map) return false;
 
     unsigned int index = hashFunction(rec.id, map->bucketCount);
 
     // Allocate memory for the new node
     HashNode *newNode = (HashNode*) malloc(sizeof(HashNode));
     if (!newNode) return false;
 
     // Copy record data into the new node
     newNode->record.id = rec.id;
     newNode->record.data = strdup(rec.data);  // Allocate and copy data string
 
     // Insert new node at the beginning of the linked list
     newNode->next = map->buckets[index];
     map->buckets[index] = newNode;
 
     return true;
 }
 
 /**
  * Retrieves a record from the hash map based on the record ID.
  *
  * This function searches through the linked list in the appropriate bucket
  * and returns a pointer to the found record, or NULL if not found.
  *
  * @param map A pointer to the HashMap.
  * @param id The ID of the record to retrieve.
  * @return A pointer to the Record if found, otherwise NULL.
  */
 Record* getRecord(HashMap *map, int id) {
     if (!map) return NULL;
 
     unsigned int index = hashFunction(id, map->bucketCount);
     HashNode *node = map->buckets[index];
 
     // Traverse the linked list to find the matching record
     while (node) {
         if (node->record.id == id) {
             return &node->record; // Return pointer to found record
         }
         node = node->next;
     }
 
     return NULL; // Record not found
 }
 
 /**
  * Deletes a record from the hash map.
  *
  * If the record is found, it is removed from the linked list and memory is freed.
  *
  * @param map A pointer to the HashMap.
  * @param id The ID of the record to delete.
  * @return True if the record was successfully deleted, false if not found.
  */
 bool deleteRecord(HashMap *map, int id) {
     if (!map) return false;
 
     unsigned int index = hashFunction(id, map->bucketCount);
     HashNode *node = map->buckets[index];
     HashNode *prev = NULL;
 
     // Traverse the linked list to find the record
     while (node) {
         if (node->record.id == id) {
             // Remove the node from the linked list
             if (prev) {
                 prev->next = node->next;
             } else {
                 map->buckets[index] = node->next;
             }
 
             // Free allocated memory
             free(node->record.data);
             free(node);
 
             return true; // Deletion successful
         }
         prev = node;
         node = node->next;
     }
 
     return false; // Record not found
 }
 