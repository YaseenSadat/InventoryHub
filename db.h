#ifndef DB_H
#define DB_H

#include "hashmap.h"
#include <sqlite3.h>

// Database structure holding SQLite connection and the in-memory cache.
typedef struct Database {
    sqlite3 *conn;
    HashMap *cache;
} Database;

// Function declarations:
int initDatabase(Database *db, const char *dbPath);
int createTable(Database *db);
int insertIntoDB(Database *db, Record rec);
int deleteFromDB(Database *db, int id);
int loadCache(Database *db);
void closeDatabase(Database *db);

#endif // DB_H
