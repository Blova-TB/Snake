#pragma once

#include <string>

class Vector2D
{
public:
    Vector2D(float x, float y) : x(x), y(y) {}
    float x;
    float y;

    Vector2D inline operator*(float scalar) const
    {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D inline operator+(const Vector2D &other) const
    {
        return Vector2D(x + other.x, y + other.y);
    }

    std::string inline toString()
    {
        return "x :" + std::to_string(x) + "\t y :" + std::to_string(y);
    }

    void inline norm()
    {
        float length = std::sqrt(x * x + y * y);

        if (length > 0.0f)
        {
            x /= length;
            y /= length;
        }
    }
};