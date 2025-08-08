#include "player.hpp"

using namespace std;

Player::Player(string name, string id)
{
    this->name = name;
    this->id = id;
    position = {0.0, 0.0, 0.0};
    velocity = {0.0, 0.0, 0.0};
}

Player::~Player()
{

}

const vector<double> Player::getPosition()
{
    return position;
}

const vector<double> Player::getVelocity()
{
    return velocity;
}

void Player::setPosition(const vector<double> &newPosition)
{
    position = newPosition;
}

void Player::setVelocity(const vector<double> &newVelocity)
{
    velocity = newVelocity;
}
