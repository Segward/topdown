#include "playerdao.hpp"
#include <functional>
#include <format>

using namespace std;

PlayerDAO::PlayerDAO(Database &db)
    : database(db)
{
    database.execute("CREATE TABLE IF NOT EXISTS players (id TEXT PRIMARY KEY, name TEXT);");
}

bool PlayerDAO::addPlayer(const string &name)
{
    hash<string> hasher;
    string id = to_string(hasher(name));
    if (nameExists(name) || idExists(id))
    {
        return false;
    }

    string query = format("INSERT INTO players (name, id) VALUES ('{}', '{}')", name, id);
    return database.execute(query);
}

vector<Player> PlayerDAO::getPlayers()
{
    vector<Player> players;
    sqlite3_stmt *stmt = database.prepare("SELECT name, id FROM players");
    if (!stmt)
        return players;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        string name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        string id = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        players.emplace_back(name, id);
    }

    sqlite3_finalize(stmt);
    return players;
}

bool PlayerDAO::idExists(const string &id)
{
    sqlite3_stmt *stmt = database.prepare("SELECT 1 FROM players WHERE id = ?");
    if (!stmt)
        return false;

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}

bool PlayerDAO::nameExists(const std::string &name)
{
    sqlite3_stmt *stmt = database.prepare("SELECT 1 FROM players WHERE name = ?");
    if (!stmt)
        return false;

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}
