#pragma once

class Vector2D
{
public:
    Vector2D(float x, float y) : x(x), y(y) {}
    float x;
    float y;

    Vector2D operator*(float scalar) const
    {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D operator+(const Vector2D &other) const
    {
        return Vector2D(x + other.x, y + other.y);
    }
};