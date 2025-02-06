#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "btree.h"
#include "hashmap.h"

// --- B-Tree Wrappers ---

// Global B-tree pointer
static BTreeNode *globalBTree = NULL;

// Initialize the global B-tree.
void init_btree() {
    if (!globalBTree) {
        globalBTree = createBTreeNode(true);
    }
}

// Insert a key into the global B-tree.
void insert_into_btree(int key) {
    if (!globalBTree) {
        init_btree();
    }
    btreeInsert(&globalBTree, key);
}

// Traverse the global B-tree and print keys (for debugging/demo).
void traverse_btree() {
    if (globalBTree) {
        btreeTraverse(globalBTree);
        printf("\n");
    }
}

// --- Hash Map Wrappers ---

// Global hash map pointer.
static HashMap *globalHashMap = NULL;

// Initialize the global hash map.
void init_hashmap() {
    if (!globalHashMap) {
        globalHashMap = createHashMap(INITIAL_BUCKET_COUNT);
    }
}

// Insert a record into the global hash map.
void insert_into_hashmap(int id, const char *data) {
    if (!globalHashMap) {
        init_hashmap();
    }
    Record rec;
    rec.id = id;
    // Make a copy of data since insertRecord will duplicate it.
    rec.data = strdup(data);
    insertRecord(globalHashMap, rec);
    // Free the temporary copy.
    free(rec.data);
}

// Retrieve a record's data from the global hash map by id.
// Returns a pointer to an internal string. (Do not free it.)
const char* get_from_hashmap(int id) {
    if (!globalHashMap) return NULL;
    Record *rec = getRecord(globalHashMap, id);
    if (rec)
        return rec->data;
    return NULL;
}

// For completeness, you could also add a cleanup function.
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
