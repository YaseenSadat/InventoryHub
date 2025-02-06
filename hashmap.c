#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

// A simple hash function (modulus of the key)
unsigned int hashFunction(int key, int bucketCount) {
    return (unsigned int) key % bucketCount;
}

HashMap* createHashMap(int bucketCount) {
    HashMap *map = (HashMap*) malloc(sizeof(HashMap));
    if (!map) return NULL;
    map->bucketCount = bucketCount;
    map->buckets = (HashNode**) calloc(bucketCount, sizeof(HashNode*));
    if (!map->buckets) {
        free(map);
        return NULL;
    }
    return map;
}

void freeHashMap(HashMap *map) {
    if (!map) return;
    for (int i = 0; i < map->bucketCount; i++) {
        HashNode *node = map->buckets[i];
        while (node) {
            HashNode *temp = node;
            node = node->next;
            free(temp->record.data);  // free allocated data
            free(temp);
        }
    }
    free(map->buckets);
    free(map);
}

bool insertRecord(HashMap *map, Record rec) {
    if (!map) return false;
    unsigned int index = hashFunction(rec.id, map->bucketCount);
    HashNode *newNode = (HashNode*) malloc(sizeof(HashNode));
    if (!newNode) return false;
    // Make a copy of the record’s data if needed.
    newNode->record.id = rec.id;
    newNode->record.data = strdup(rec.data);  
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
    return true;
}

Record* getRecord(HashMap *map, int id) {
    if (!map) return NULL;
    unsigned int index = hashFunction(id, map->bucketCount);
    HashNode *node = map->buckets[index];
    while (node) {
        if (node->record.id == id) {
            return &node->record;
        }
        node = node->next;
    }
    return NULL;
}

bool deleteRecord(HashMap *map, int id) {
    if (!map) return false;
    unsigned int index = hashFunction(id, map->bucketCount);
    HashNode *node = map->buckets[index];
    HashNode *prev = NULL;
    while (node) {
        if (node->record.id == id) {
            if (prev) {
                prev->next = node->next;
            } else {
                map->buckets[index] = node->next;
            }
            free(node->record.data);
            free(node);
            return true;
        }
        prev = node;
        node = node->next;
    }
    return false;
}
