#pragma once

#include <string>

class UserPlayer
{
private:
    int id;
    std::string name;
    int score;
    int health;
    // ...

public:
    UserPlayer(const int id, const std::string &name);
    int getId() const;
    std::string getName() const;
    int getScore() const;
    int getHealth() const;
};
