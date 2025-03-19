/**
 * query_parser.c
 *
 * A basic query parser for handling simple database commands.
 *
 * This module processes user input queries and executes corresponding
 * database operations, including:
 * - `INSERT <id> <data>` : Inserts a record into the database.
 * - `SELECT <id>`        : Retrieves a record from the database.
 * - `DELETE <id>`        : Deletes a record from the database.
 *
 * The parser uses basic string parsing functions to extract command arguments.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "query_parser.h"
 #include "hashmap.h"
 
 /**
  * Processes a user query and performs the corresponding database operation.
  *
  * Supported commands:
  * - `INSERT <id> <data>` : Adds a new record to the database.
  * - `SELECT <id>`        : Retrieves and displays a record.
  * - `DELETE <id>`        : Removes a record from the database.
  *
  * @param db A pointer to the `Database` structure.
  * @param query The user-provided query string.
  */
 void processQuery(Database *db, const char *query) {
     // Handle `INSERT` command
     if (strncmp(query, "INSERT", 6) == 0) {
         // Expected format: INSERT <id> <data>
         int id;
         char data[256];
 
         // Extract ID and data from the query string
         if (sscanf(query + 6, "%d %[^\n]", &id, data) == 2) {
             Record rec;
             rec.id = id;
             rec.data = data;  // Temporary pointer; insertIntoDB will duplicate the string
 
             // Attempt to insert record into the database
             if (insertIntoDB(db, rec) == SQLITE_OK)
                 printf("Record inserted: id=%d, data=%s\n", id, data);
             else
                 printf("Error inserting record.\n");
         } else {
             printf("Invalid INSERT syntax.\n");
         }
     } 
     // Handle `SELECT` command
     else if (strncmp(query, "SELECT", 6) == 0) {
         // Expected format: SELECT <id>
         int id;
 
         // Extract ID from the query string
         if (sscanf(query + 6, "%d", &id) == 1) {
             // Retrieve record from the in-memory cache
             Record *rec = getRecord(db->cache, id);
             if (rec)
                 printf("Record found: id=%d, data=%s\n", rec->id, rec->data);
             else
                 printf("Record not found.\n");
         } else {
             printf("Invalid SELECT syntax.\n");
         }
     } 
     // Handle `DELETE` command
     else if (strncmp(query, "DELETE", 6) == 0) {
         // Expected format: DELETE <id>
         int id;
 
         // Extract ID from the query string
         if (sscanf(query + 6, "%d", &id) == 1) {
             // Attempt to delete the record from the database
             if (deleteFromDB(db, id) == SQLITE_OK)
                 printf("Record deleted: id=%d\n", id);
             else
                 printf("Error deleting record.\n");
         } else {
             printf("Invalid DELETE syntax.\n");
         }
     } 
     // Handle unknown commands
     else {
         printf("Unknown command.\n");
     }
 }
 