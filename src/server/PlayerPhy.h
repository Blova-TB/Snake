#pragma once

#include <string>
#include <vector>

#include "../shared/Vector2D.h"

class PlayerPhy
{
private:
    int id;
    Vector2D position;
    Vector2D vitesse;

public:
    PlayerPhy(int id);

    int getId() const;
    Vector2D getPosition() const;
    Vector2D getVitesse() const;

    void setPosition(const Vector2D &pos);
    void setVitesse(const Vector2D &vit);
};
