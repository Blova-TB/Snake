#pragma once

#include <cstdint>
#include "PacketTemplate.h"

// --------------------------------------- Client to Server ---------------------------------------

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

struct InputRecord
{
    uint64_t timestamp;
    uint8_t action_mask;
};
void read_from(ByteBuffer buf, InputRecord &pk)
{
    read_from(buf, pk.timestamp);
    read_from(buf, pk.action_mask);
}
void write_to(std::vector<uint8_t> &buf, const InputRecord &pk)
{
    write_to(buf, pk.timestamp);
    write_to(buf, pk.action_mask);
}

struct PktInputs
{
    static constexpr uint8_t TAG = 0x03;
    std::vector<InputRecord> inputs;
};
void read_from(ByteBuffer buf, PktInputs &pk)
{
    read_from(buf, pk.inputs);
}
void write_to(std::vector<uint8_t> &buf, const PktInputs &pk)
{
    write_to(buf, pk.inputs);
}

struct PktLogIn
{
    static constexpr uint8_t TAG = 0x01;
    std::string pseudo;
};
void read_from(ByteBuffer buf, PktLogIn &pk)
{
    read_from(buf, pk.pseudo);
}
void write_to(std::vector<uint8_t> &buf, const PktLogIn &pk)
{
    write_to(buf, pk.pseudo);
}

struct PktLogOut
{
    static constexpr uint8_t TAG = 0x02;
};
void read_from(ByteBuffer buf, PktLogOut &pk)
{
}
void write_to(std::vector<uint8_t> &buf, const PktLogOut &pk)
{
}


using PacketPayload = std::variant<PktLogIn, PktLogOut, PktInputs>;

struct GamePacket
{
    uint32_t player_id;
    uint8_t tag;
    PacketPayload payload;

    std::vector<uint8_t> to_bytes() const
    {
        std::vector<uint8_t> buf;

        write_to(buf, player_id);
        write_to(buf, tag);

        std::visit([&buf](const auto& pkt) { 
            write_to(buf, pkt); 
        }, payload);

        return buf;
    }

    static GamePacket try_from_bytes(const std::vector<uint8_t> &raw_data)
    {
        ByteBuffer buf{raw_data, 0};

        GamePacket packet;
        // TODO
        // packet.player_id = read_from<PlayerId>(buf);
        // packet.tag = read_from<uint8_t>(buf);

        // switch (packet.tag)
        // {
        // case PktLogIn::TAG:
        //     packet.payload = PktLogIn::read(buf);
        //     break;
        // case PktLogOut::TAG:
        //     packet.payload = PktLogOut::read(buf);
        //     break;
        // case PktInputs::TAG:
        //     packet.payload = PktInputs::read(buf);
        //     break;
        // case PktMessage::TAG:
        //     packet.payload = PktMessage::read(buf);
        //     break;
        // default:
        //     throw std::runtime_error("Tag de packet inconnu !");
        // }
        // return packet;
    }
};