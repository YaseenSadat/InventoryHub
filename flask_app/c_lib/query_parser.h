/**
 * query_parser.h
 *
 * Header file for the query processing module.
 *
 * This module provides functionality to interpret and execute simple SQL-like
 * commands on an in-memory database. It supports:
 * - `INSERT <id> <data>` : Adds a new record to the database.
 * - `SELECT <id>`        : Retrieves and displays a record.
 * - `DELETE <id>`        : Removes a record from the database.
 *
 * Usage:
 * - Call `processQuery(db, query)` to execute a user-provided query string.
 */

 #ifndef QUERY_PARSER_H
 #define QUERY_PARSER_H
 
 #include "db.h"
 
 /**
  * Processes a query string and executes the corresponding database operation.
  *
  * The function recognizes three command types:
  * - `INSERT <id> <data>` : Inserts a record into the database.
  * - `SELECT <id>`        : Retrieves and prints a record.
  * - `DELETE <id>`        : Deletes a record from the database.
  *
  * If the command is not recognized, an error message is displayed.
  *
  * @param db A pointer to the `Database` structure.
  * @param query The user-provided query string.
  */
 void processQuery(Database *db, const char *query);
 
 #endif // QUERY_PARSER_H
 