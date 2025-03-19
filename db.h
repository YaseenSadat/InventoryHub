/**
 * db.h
 *
 * Header file for the database module.
 *
 * This module provides an interface for interacting with an SQLite database
 * while utilizing an in-memory hash map cache to optimize performance.
 *
 * Features:
 * - Establishes and manages an SQLite database connection.
 * - Uses an in-memory cache (hash map) to store frequently accessed records.
 * - Supports insertion, deletion, and retrieval of records.
 * - Loads existing records into the cache on startup.
 * - Ensures proper cleanup of resources upon closure.
 */

 #ifndef DB_H
 #define DB_H
 
 #include "hashmap.h"  // HashMap for in-memory caching
 #include <sqlite3.h>   // SQLite database management
 
 /**
  * Database structure for managing SQLite connection and in-memory cache.
  *
  * This structure holds:
  * - `conn` : Pointer to the SQLite database connection.
  * - `cache`: Pointer to an in-memory hash map cache for fast record access.
  */
 typedef struct Database {
     sqlite3 *conn;   ///< SQLite database connection pointer.
     HashMap *cache;  ///< Pointer to the in-memory cache for quick lookups.
 } Database;
 
 /**
  * Initializes the SQLite database and sets up the in-memory cache.
  *
  * This function opens the database connection and initializes the cache.
  * It should be called before performing any database operations.
  *
  * @param db A pointer to the Database structure.
  * @param dbPath The file path of the SQLite database.
  * @return SQLITE_OK (0) on success, or an SQLite error code on failure.
  */
 int initDatabase(Database *db, const char *dbPath);
 
 /**
  * Creates the database table if it does not already exist.
  *
  * The table structure:
  * - `id`   : INTEGER PRIMARY KEY
  * - `data` : TEXT (Not NULL)
  *
  * @param db A pointer to the Database structure.
  * @return SQLITE_OK (0) on success, or an SQLite error code on failure.
  */
 int createTable(Database *db);
 
 /**
  * Inserts a record into both the SQLite database and the in-memory cache.
  *
  * @param db A pointer to the Database structure.
  * @param rec The record to be inserted.
  * @return SQLITE_OK (0) on success, or an SQLite error code on failure.
  */
 int insertIntoDB(Database *db, Record rec);
 
 /**
  * Deletes a record from both the SQLite database and the in-memory cache.
  *
  * @param db A pointer to the Database structure.
  * @param id The ID of the record to be deleted.
  * @return SQLITE_OK (0) on success, or an SQLite error code on failure.
  */
 int deleteFromDB(Database *db, int id);
 
 /**
  * Loads all records from the SQLite database into the in-memory cache.
  *
  * This function is typically called on startup to populate the cache
  * with the existing database records for quick access.
  *
  * @param db A pointer to the Database structure.
  * @return SQLITE_OK (0) on success, or an SQLite error code on failure.
  */
 int loadCache(Database *db);
 
 /**
  * Closes the SQLite database connection and frees the in-memory cache.
  *
  * This function should be called when the database is no longer needed
  * to ensure proper resource cleanup and avoid memory leaks.
  *
  * @param db A pointer to the Database structure.
  */
 void closeDatabase(Database *db);
 
 #endif // DB_H
 