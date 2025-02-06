#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include "query_parser.h"
#include "btree.h"  // Include if you want to demonstrate B-tree usage

#define DB_PATH "inmemory.db"
#define QUERY_BUFFER_SIZE 512

int main() {
    Database db;
    if (initDatabase(&db, DB_PATH) != SQLITE_OK) {
        fprintf(stderr, "Failed to initialize database.\n");
        return EXIT_FAILURE;
    }
    
    if (createTable(&db) != SQLITE_OK) {
        fprintf(stderr, "Failed to create table.\n");
        closeDatabase(&db);
        return EXIT_FAILURE;
    }
    
    if (loadCache(&db) != SQLITE_OK) {
        fprintf(stderr, "Failed to load cache.\n");
        closeDatabase(&db);
        return EXIT_FAILURE;
    }
    
    printf("Custom In-Memory DB is ready. Enter commands (INSERT, SELECT, DELETE) or type EXIT to quit.\n");
    
    char query[QUERY_BUFFER_SIZE];
    while (1) {
        printf(">> ");
        if (!fgets(query, QUERY_BUFFER_SIZE, stdin))
            break;
        // Remove trailing newline.
        query[strcspn(query, "\n")] = 0;
        if (strcasecmp(query, "EXIT") == 0)
            break;
        
        processQuery(&db, query);
    }
    
    closeDatabase(&db);
    printf("Database closed. Goodbye!\n");
    return EXIT_SUCCESS;
}
