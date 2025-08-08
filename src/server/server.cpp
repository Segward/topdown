#include "database/database.hpp"

int main()
{
    Database db("test.sqlite");
    db.execute("CREATE TABLE IF NOT EXISTS players (id INTEGER PRIMARY KEY, name TEXT);");

    return 0;
}