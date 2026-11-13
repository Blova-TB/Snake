#pragma once

#include "LidSerialization.h"

#include <cstdint>
#include <vector>
#include <string>
#include <variant>


//  CLIENT TO SERVER

struct InputRecord
{
    uint64_t timestamp;
    uint8_t action_mask;
};

enum class InputFlags : uint8_t
{
    None = 0,
    Up = 1 << 0,
    Down = 1 << 1,
    Left = 1 << 2,
    Right = 1 << 3,
    Action = 1 << 4
};

inline bool inputContain(uint8_t input, InputFlags flag)
{

    if (flag == InputFlags::None)
    {
        return input == 0;
    }

    return (input & static_cast<uint8_t>(flag)) != 0;
}

struct PktLogIn
{
    static constexpr uint8_t TAG = 0x01;
    std::string pseudo;
};

struct PktLogOut
{
    static constexpr uint8_t TAG = 0x02;
};

struct PktInputs
{
    static constexpr uint8_t TAG = 0x03;
    std::vector<InputRecord> inputs;
};

using ClientToServerPayload = std::variant<PktInputs, PktLogIn, PktLogOut>;
using ClientToServerPacket = GamePacket<ClientToServerPayload>;

// SERVER TO CLIENT ------------------------------------------

struct playerSnapshot
{
    uint8_t id;
    uint32_t px; // TODO -> change for lighter
    uint32_t py;
    uint32_t vx; // TODO -> same (whith an angle?)
    uint32_t vy;
};

struct PktSnapshot
{
    static constexpr uint8_t TAG = 0x01;
    std::vector<playerSnapshot> playersPosAndSpeed;
};

using ServerToClientPayload = std::variant<PktSnapshot>;
using ServerToClientPacket = GamePacket<ServerToClientPayload>;