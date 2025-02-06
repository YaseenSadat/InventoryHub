#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "query_parser.h"
#include "hashmap.h"

// Very basic query parser that checks for keywords.
void processQuery(Database *db, const char *query) {
    if (strncmp(query, "INSERT", 6) == 0) {
        // Expected format: INSERT <id> <data>
        int id;
        char data[256];
        if (sscanf(query + 6, "%d %[^\n]", &id, data) == 2) {
            Record rec;
            rec.id = id;
            rec.data = data;  // temporary pointer; insertIntoDB will duplicate the string
            if (insertIntoDB(db, rec) == SQLITE_OK)
                printf("Record inserted: id=%d, data=%s\n", id, data);
            else
                printf("Error inserting record.\n");
        } else {
            printf("Invalid INSERT syntax.\n");
        }
    } else if (strncmp(query, "SELECT", 6) == 0) {
        // Expected format: SELECT <id>
        int id;
        if (sscanf(query + 6, "%d", &id) == 1) {
            Record *rec = getRecord(db->cache, id);
            if (rec)
                printf("Record found: id=%d, data=%s\n", rec->id, rec->data);
            else
                printf("Record not found.\n");
        } else {
            printf("Invalid SELECT syntax.\n");
        }
    } else if (strncmp(query, "DELETE", 6) == 0) {
        // Expected format: DELETE <id>
        int id;
        if (sscanf(query + 6, "%d", &id) == 1) {
            if (deleteFromDB(db, id) == SQLITE_OK)
                printf("Record deleted: id=%d\n", id);
            else
                printf("Error deleting record.\n");
        } else {
            printf("Invalid DELETE syntax.\n");
        }
    } else {
        printf("Unknown command.\n");
    }
}
