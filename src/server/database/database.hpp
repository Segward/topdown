#pragma once

#include <string>
#include <sqlite3.h>

class Database
{
public:
    Database(const std::string &path);
    ~Database();
    bool execute(const std::string &query);
    void output();

private:
    sqlite3 *db;
};
