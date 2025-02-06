#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

// Initialize the SQLite database and create the in-memory cache.
int initDatabase(Database *db, const char *dbPath) {
    int rc = sqlite3_open(dbPath, &db->conn);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db->conn));
        return rc;
    }
    db->cache = createHashMap(INITIAL_BUCKET_COUNT);
    if (!db->cache) {
        fprintf(stderr, "Cannot create in-memory cache\n");
        return -1;
    }
    return SQLITE_OK;
}

// Create a table if it does not exist.
int createTable(Database *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS records ("
                      "id INTEGER PRIMARY KEY, "
                      "data TEXT NOT NULL);";
    char *errMsg = NULL;
    int rc = sqlite3_exec(db->conn, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error (createTable): %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return rc;
}

// Insert a record into SQL and the in-memory cache.
int insertIntoDB(Database *db, Record rec) {
    // Insert into SQLite.
    const char *sql = "INSERT INTO records (id, data) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db->conn, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db->conn));
        return rc;
    }
    sqlite3_bind_int(stmt, 1, rec.id);
    sqlite3_bind_text(stmt, 2, rec.data, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db->conn));
        sqlite3_finalize(stmt);
        return rc;
    }
    sqlite3_finalize(stmt);

    // Insert into the in-memory hash map cache.
    if (!insertRecord(db->cache, rec)) {
        fprintf(stderr, "Failed to insert into in-memory cache\n");
        return -1;
    }
    return SQLITE_OK;
}

// Delete a record from both SQLite and the cache.
int deleteFromDB(Database *db, int id) {
    const char *sql = "DELETE FROM records WHERE id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db->conn, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare delete statement: %s\n", sqlite3_errmsg(db->conn));
        return rc;
    }
    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Delete execution failed: %s\n", sqlite3_errmsg(db->conn));
        sqlite3_finalize(stmt);
        return rc;
    }
    sqlite3_finalize(stmt);

    // Remove from in-memory cache.
    if (!deleteRecord(db->cache, id)) {
        fprintf(stderr, "Failed to delete from in-memory cache (record may not exist)\n");
    }
    return SQLITE_OK;
}

// On startup, load all records from SQLite into the hash map.
int loadCache(Database *db) {
    const char *sql = "SELECT id, data FROM records;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db->conn, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare loadCache statement: %s\n", sqlite3_errmsg(db->conn));
        return rc;
    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        Record rec;
        rec.id = sqlite3_column_int(stmt, 0);
        const unsigned char *text = sqlite3_column_text(stmt, 1);
        rec.data = strdup((const char*) text); // duplicate string for our own memory
        if (!insertRecord(db->cache, rec)) {
            fprintf(stderr, "Failed to insert record into cache during load\n");
        }
        // Free the duplicated string in rec since insertRecord makes its own copy.
        free(rec.data);
    }
    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

// Close SQLite connection and free cache.
void closeDatabase(Database *db) {
    if (db->cache) {
        freeHashMap(db->cache);
        db->cache = NULL;
    }
    if (db->conn) {
        sqlite3_close(db->conn);
        db->conn = NULL;
    }
}
