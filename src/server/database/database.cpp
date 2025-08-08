#include <iostream>
#include "database.hpp"

Database::Database(const std::string &path)
{
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK)
    {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }
}

Database::~Database()
{
    sqlite3_close(db);
}

bool Database::execute(const std::string &query)
{
    char *errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "Error executing query: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

void Database::output()
{
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "SELECT * FROM players;", -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
            std::cout << "Player ID: " << id << ", Name: " << (name ? name : "Unknown") << std::endl;
        }
        sqlite3_finalize(stmt);
    }
}
