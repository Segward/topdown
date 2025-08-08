#pragma once
#include <vector>

using namespace std;

class Player
{
public:
    Player(string name, string id);
    vector<double> getPosition() const;
    vector<double> getVelocity() const;
    void setPosition(const vector<double> &newPosition);
    void setVelocity(const vector<double> &newVelocity);
    string getName() const;
    string getId() const;

private:
    vector<double> position;
    vector<double> velocity;
    const string name;
    const string id;
};
