#ifndef INVENTORY_STRUCTURES_H
#define INVENTORY_STRUCTURES_H

// B-tree functions
void init_btree();
void insert_into_btree(int key);
void traverse_btree();

// Hash map functions
void init_hashmap();
void insert_into_hashmap(int id, const char *data);
const char* get_from_hashmap(int id);

// Cleanup
void free_structures();

#endif // INVENTORY_STRUCTURES_H
