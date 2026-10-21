#pragma once

#include <cstdint>
#include <vector>
#include <string>

enum class MsgType : uint8_t
{
    None = 0,
    Connect = 1,
    Disconnect = 2,
    Input = 3,
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

// ----------------------------------------------------

struct InputRecord
{
    uint64_t timestamp;
    uint8_t action_mask;
};
// void read_from(ByteBuffer &buf, InputRecord &pk)
// {
//     read_from(buf, pk.timestamp);
//     read_from(buf, pk.action_mask);
// }
// void write_to(std::vector<uint8_t> &buf, const InputRecord &pk)
// {
//     write_to(buf, pk.timestamp);
//     write_to(buf, pk.action_mask);
// }

struct PktInputs
{
    static constexpr uint8_t TAG = 0x03;
    std::vector<InputRecord> inputs;
};
// void read_from(ByteBuffer &buf, PktInputs &pk)
// {
//     read_from(buf, pk.inputs);
// }
// void write_to(std::vector<uint8_t> &buf, const PktInputs &pk)
// {
//     write_to(buf, pk.inputs);
// }

struct PktLogIn
{
    static constexpr uint8_t TAG = 0x01;
    std::string pseudo;
};
// void read_from(ByteBuffer &buf, PktLogIn &pk)
// {
//     read_from(buf, pk.pseudo);
// }
// void write_to(std::vector<uint8_t> &buf, const PktLogIn &pk)
// {
//     write_to(buf, pk.pseudo);
// }

struct PktLogOut
{
    static constexpr uint8_t TAG = 0x02;
};
// void read_from(ByteBuffer &buf, PktLogOut &pk)
// {
// }
// void write_to(std::vector<uint8_t> &buf, const PktLogOut &pk)
// {
// }