#include "database/dbconnection.hpp"

int main()
{
    DbConnection db("test.sqlite");
    db.execute("CREATE TABLE IF NOT EXISTS players (id INTEGER PRIMARY KEY, name TEXT);");

    return 0;
}