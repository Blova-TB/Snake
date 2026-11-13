#include "OtherPlayer.h"

OtherPlayer::OtherPlayer(const int id, const std::string &name) : id(id), name(name) {}

int OtherPlayer::getId() const
{
    return id;
}

std::string OtherPlayer::getName() const
{
    return name;
}
