#pragma once

#include <string>

class OtherPlayer
{
private:
    int id;
    std::string name;
    // ...

public:
    OtherPlayer(const int id, const std::string &name);
    int getId() const;
    std::string getName() const;
};
