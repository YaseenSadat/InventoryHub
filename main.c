/**
 * main.c
 *
 * Entry point for the custom in-memory database application.
 *
 * This program initializes an SQLite database with an in-memory cache and
 * allows users to interact with it via simple text-based commands.
 *
 * Features:
 * - Uses SQLite as a persistent backend.
 * - Implements an in-memory hash map to speed up record lookups.
 * - Supports INSERT, SELECT, and DELETE operations via query parsing.
 * - Optionally integrates a B-tree for ordered data operations.
 *
 * Usage:
 * - Start the program and enter SQL-like commands (`INSERT`, `SELECT`, `DELETE`).
 * - Type `EXIT` to close the database and terminate the program.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "db.h"
 #include "query_parser.h"
 #include "btree.h"  // Include if you want to demonstrate B-tree usage
 
 /**
  * Path to the SQLite database file.
  *
  * - The database is stored in a file named `inmemory.db`.
  * - Modify this path if a different storage location is needed.
  */
 #define DB_PATH "inmemory.db"
 
 /**
  * Buffer size for storing user input queries.
  *
  * - The program reads up to 512 characters per query.
  * - If a query exceeds this length, it will be truncated.
  */
 #define QUERY_BUFFER_SIZE 512
 
 /**
  * Main function - entry point of the program.
  *
  * - Initializes the database and sets up the in-memory cache.
  * - Waits for user input and processes SQL-like queries.
  * - Cleans up resources upon exit.
  *
  * @return EXIT_SUCCESS on normal termination, EXIT_FAILURE on errors.
  */
 int main() {
     Database db;
 
     // Initialize the database
     if (initDatabase(&db, DB_PATH) != SQLITE_OK) {
         fprintf(stderr, "Failed to initialize database.\n");
         return EXIT_FAILURE;
     }
 
     // Create the required database table
     if (createTable(&db) != SQLITE_OK) {
         fprintf(stderr, "Failed to create table.\n");
         closeDatabase(&db);
         return EXIT_FAILURE;
     }
 
     // Load existing records from SQLite into the in-memory cache
     if (loadCache(&db) != SQLITE_OK) {
         fprintf(stderr, "Failed to load cache.\n");
         closeDatabase(&db);
         return EXIT_FAILURE;
     }
 
     printf("Custom In-Memory DB is ready. Enter commands (INSERT, SELECT, DELETE) or type EXIT to quit.\n");
 
     // Input buffer for user queries
     char query[QUERY_BUFFER_SIZE];
 
     // Main loop for handling user input
     while (1) {
         printf(">> ");  // Command prompt
 
         // Read user input
         if (!fgets(query, QUERY_BUFFER_SIZE, stdin))
             break;
 
         // Remove trailing newline from input
         query[strcspn(query, "\n")] = 0;
 
         // Exit condition
         if (strcasecmp(query, "EXIT") == 0)
             break;
 
         // Process user query
         processQuery(&db, query);
     }
 
     // Cleanup: Close database connection and free resources
     closeDatabase(&db);
     printf("Database closed. Goodbye!\n");
 
     return EXIT_SUCCESS;
 }
 