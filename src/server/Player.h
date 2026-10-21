#pragma once

#include <string>

class Player
{
private:
    int id;
    std::string name;
    int score;
    int health;
    // ...

public:
    Player(int id, const std::string &name);
    int getId() const;
    std::string getName() const;
    int getScore() const;
    int getHealth() const;
};
