#pragma once
#include <vector>

using namespace std;

class Player
{
public:
    Player();
    ~Player();
    const vector<double> getPosition();
    const vector<double> getVelocity();
    void setPosition(const vector<double> &newPosition);
    void setVelocity(const vector<double> &newVelocity);

private:
    vector<double> position;
    vector<double> velocity;
};
