/**
 * db.c
 *
 * Implementation of a SQLite database with an in-memory caching system.
 *
 * This module provides functions to:
 * - Initialize the SQLite database and create an in-memory cache.
 * - Create a database table for storing records.
 * - Insert, delete, and retrieve records from both SQLite and the cache.
 * - Load all records from SQLite into the cache at startup.
 * - Close the database and free allocated resources.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "db.h"
 
 /**
  * Initializes the SQLite database and creates an in-memory cache.
  *
  * @param db A pointer to the Database structure.
  * @param dbPath The file path of the SQLite database.
  * @return SQLITE_OK (0) on success, or an SQLite error code on failure.
  */
 int initDatabase(Database *db, const char *dbPath) {
     // Open SQLite database connection
     int rc = sqlite3_open(dbPath, &db->conn);
     if (rc != SQLITE_OK) {
         fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db->conn));
         return rc;
     }
 
     // Initialize in-memory hash map cache
     db->cache = createHashMap(INITIAL_BUCKET_COUNT);
     if (!db->cache) {
         fprintf(stderr, "Cannot create in-memory cache\n");
         return -1;
     }
 
     return SQLITE_OK;
 }
 
 /**
  * Creates the database table if it does not already exist.
  *
  * The table structure:
  * - `id`   : Integer primary key.
  * - `data` : Text data associated with the record.
  *
  * @param db A pointer to the Database structure.
  * @return SQLITE_OK (0) on success, or an SQLite error code on failure.
  */
 int createTable(Database *db) {
     const char *sql = "CREATE TABLE IF NOT EXISTS records ("
                       "id INTEGER PRIMARY KEY, "
                       "data TEXT NOT NULL);";
     char *errMsg = NULL;
 
     // Execute SQL statement
     int rc = sqlite3_exec(db->conn, sql, 0, 0, &errMsg);
     if (rc != SQLITE_OK) {
         fprintf(stderr, "SQL error (createTable): %s\n", errMsg);
         sqlite3_free(errMsg);
     }
 
     return rc;
 }
 
 /**
  * Inserts a record into both the SQLite database and the in-memory cache.
  *
  * @param db A pointer to the Database structure.
  * @param rec The record to be inserted.
  * @return SQLITE_OK (0) on success, or an SQLite error code on failure.
  */
 int insertIntoDB(Database *db, Record rec) {
     const char *sql = "INSERT INTO records (id, data) VALUES (?, ?);";
     sqlite3_stmt *stmt;
 
     // Prepare SQL statement for execution
     int rc = sqlite3_prepare_v2(db->conn, sql, -1, &stmt, NULL);
     if (rc != SQLITE_OK) {
         fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db->conn));
         return rc;
     }
 
     // Bind values to the SQL statement
     sqlite3_bind_int(stmt, 1, rec.id);
     sqlite3_bind_text(stmt, 2, rec.data, -1, SQLITE_STATIC);
 
     // Execute SQL statement
     rc = sqlite3_step(stmt);
     if (rc != SQLITE_DONE) {
         fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db->conn));
         sqlite3_finalize(stmt);
         return rc;
     }
 
     sqlite3_finalize(stmt);
 
     // Insert into the in-memory hash map cache
     if (!insertRecord(db->cache, rec)) {
         fprintf(stderr, "Failed to insert into in-memory cache\n");
         return -1;
     }
 
     return SQLITE_OK;
 }
 
 /**
  * Deletes a record from both the SQLite database and the in-memory cache.
  *
  * @param db A pointer to the Database structure.
  * @param id The ID of the record to be deleted.
  * @return SQLITE_OK (0) on success, or an SQLite error code on failure.
  */
 int deleteFromDB(Database *db, int id) {
     const char *sql = "DELETE FROM records WHERE id = ?;";
     sqlite3_stmt *stmt;
 
     // Prepare SQL delete statement
     int rc = sqlite3_prepare_v2(db->conn, sql, -1, &stmt, NULL);
     if (rc != SQLITE_OK) {
         fprintf(stderr, "Failed to prepare delete statement: %s\n", sqlite3_errmsg(db->conn));
         return rc;
     }
 
     // Bind the ID value to the delete statement
     sqlite3_bind_int(stmt, 1, id);
 
     // Execute SQL delete statement
     rc = sqlite3_step(stmt);
     if (rc != SQLITE_DONE) {
         fprintf(stderr, "Delete execution failed: %s\n", sqlite3_errmsg(db->conn));
         sqlite3_finalize(stmt);
         return rc;
     }
 
     sqlite3_finalize(stmt);
 
     // Remove from in-memory cache
     if (!deleteRecord(db->cache, id)) {
         fprintf(stderr, "Failed to delete from in-memory cache (record may not exist)\n");
     }
 
     return SQLITE_OK;
 }
 
 /**
  * Loads all records from the SQLite database into the in-memory cache.
  *
  * This function is typically called on startup to populate the cache
  * with the existing database records.
  *
  * @param db A pointer to the Database structure.
  * @return SQLITE_OK (0) on success, or an SQLite error code on failure.
  */
 int loadCache(Database *db) {
     const char *sql = "SELECT id, data FROM records;";
     sqlite3_stmt *stmt;
 
     // Prepare SQL select statement
     int rc = sqlite3_prepare_v2(db->conn, sql, -1, &stmt, NULL);
     if (rc != SQLITE_OK) {
         fprintf(stderr, "Failed to prepare loadCache statement: %s\n", sqlite3_errmsg(db->conn));
         return rc;
     }
 
     // Iterate through query results and insert into cache
     while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
         Record rec;
         rec.id = sqlite3_column_int(stmt, 0);
 
         // Duplicate text data from SQLite to ensure proper memory allocation
         const unsigned char *text = sqlite3_column_text(stmt, 1);
         rec.data = strdup((const char*) text);
 
         if (!insertRecord(db->cache, rec)) {
             fprintf(stderr, "Failed to insert record into cache during load\n");
         }
 
         // Free the duplicated string since insertRecord makes its own copy
         free(rec.data);
     }
 
     sqlite3_finalize(stmt);
     return SQLITE_OK;
 }
 
 /**
  * Closes the SQLite database connection and frees the in-memory cache.
  *
  * @param db A pointer to the Database structure.
  */
 void closeDatabase(Database *db) {
     // Free the in-memory hash map cache
     if (db->cache) {
         freeHashMap(db->cache);
         db->cache = NULL;
     }
 
     // Close SQLite database connection
     if (db->conn) {
         sqlite3_close(db->conn);
         db->conn = NULL;
     }
 }
 