#include "PlayerPhy.h"

PlayerPhy::PlayerPhy(int id) : id(id), position(0, 0), vitesse(0, 0) {}

int PlayerPhy::getId() const
{
    return id;
}

Vector2D PlayerPhy::getPosition() const
{
    return position;
}

Vector2D PlayerPhy::getVitesse() const
{
    return vitesse;
}

void PlayerPhy::setPosition(const Vector2D &pos)
{
    position = pos;
}

void PlayerPhy::setVitesse(const Vector2D &vit)
{
    vitesse = vit;
}