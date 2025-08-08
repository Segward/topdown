#include "dao/playerdao.hpp"
#include "database/database.hpp"
#include <iostream>

using namespace std;

int main()
{
    Database db("test.sqlite");
    PlayerDAO playerDAO(db);

    playerDAO.addPlayer("Alice");
    playerDAO.addPlayer("Bob");

    vector<Player> players = playerDAO.getPlayers();

    for (const auto &player : players)
    {
        string name = player.getName();
        string id = player.getId();
        cout << "Player ID: " << id << ", Name: " << name << endl;
    }

    return 0;
}