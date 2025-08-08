#pragma once

#include <string>
#include <sqlite3.h>

using namespace std;

class Database
{
public:
    Database(const string &path);
    ~Database();
    bool execute(const string &query);
    sqlite3_stmt *prepare(const string &query);

private:
    sqlite3 *db;
};
