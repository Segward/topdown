#pragma once

#include <string>
#include <vector>
#include "../database/database.hpp"
#include "../../shared/model/player.hpp"

using namespace std;

class PlayerDAO
{
public:
    PlayerDAO(Database &db);
    bool addPlayer(const std::string &name);
    vector<Player> getPlayers();

private:
    Database &database;
    bool idExists(const std::string &id);
    bool nameExists(const std::string &name);
};
