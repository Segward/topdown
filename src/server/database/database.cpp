#include <iostream>
#include "database.hpp"

using namespace std;

Database::Database(const string &path)
{
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK)
    {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
    }
}

Database::~Database()
{
    sqlite3_close(db);
}

bool Database::execute(const string &query)
{
    char *errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        cerr << "Error executing query: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

sqlite3_stmt *Database::prepare(const string &query)
{
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return stmt;
}