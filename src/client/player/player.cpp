#include "player.hpp"

using namespace std;

Player::Player(string name, string id) : name(name), id(id)
{
    position = {0.0, 0.0, 0.0};
    velocity = {0.0, 0.0, 0.0};
}

vector<double> Player::getPosition() const { return position; }
vector<double> Player::getVelocity() const { return velocity; }

void Player::setPosition(const vector<double> &newPosition)
{
    position = newPosition;
}

void Player::setVelocity(const vector<double> &newVelocity)
{
    velocity = newVelocity;
}

string Player::getName() const { return name; }
string Player::getId() const { return id; }