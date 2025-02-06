#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>

#define INITIAL_BUCKET_COUNT 101

// Define a record structure. Adjust fields as needed.
typedef struct Record {
    int id;          // Unique identifier
    char *data;      // Example data (dynamically allocated)
} Record;

// Define a hash map node for the linked list (separate chaining).
typedef struct HashNode {
    Record record;
    struct HashNode *next;
} HashNode;

// Define the hash map structure.
typedef struct HashMap {
    int bucketCount;
    HashNode **buckets;
} HashMap;

// Function declarations:
HashMap* createHashMap(int bucketCount);
void freeHashMap(HashMap *map);
bool insertRecord(HashMap *map, Record rec);
Record* getRecord(HashMap *map, int id);
bool deleteRecord(HashMap *map, int id);
unsigned int hashFunction(int key, int bucketCount);

#endif // HASHMAP_H
