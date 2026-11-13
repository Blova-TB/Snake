#include "UserPlayer.h"

UserPlayer::UserPlayer(const int id, const std::string &name) : id(id), name(name), score(0), health(100) {}

int UserPlayer::getId() const
{
    return id;
}

std::string UserPlayer::getName() const
{
    return name;
}

int UserPlayer::getScore() const
{
    return score;
}
