#include "Player.h"

Player::Player(int id, const std::string &name) : id(id), name(name), score(0), health(100) {}

int Player::getId() const
{
    return id;
}

std::string Player::getName() const
{
    return name;
}

int Player::getScore() const
{
    return score;
}
