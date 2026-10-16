#pragma once

#include <cstdint>
#include "PacketTemplate.h"


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

struct GamePacket {
    uint32_t player_id;
    PacketPayload payload;
};

void write_to(std::vector<uint8_t>& buf, const GamePacket& packet) {
    
    write_to(buf, packet.player_id);
    
    std::visit([&buf](const auto& pkt) {
        using PktType = std::decay_t<decltype(pkt)>; 
        write_to(buf, PktType::TAG); 
        write_to(buf, pkt);
    }, packet.payload);
}

GamePacket read_packet(const uint8_t* raw_data, size_t length) {
    ByteBuffer buf{raw_data, length, 0};
    GamePacket packet;

    read_from(buf, packet.player_id);
    
    uint8_t tag;
    read_from(buf, tag);

    switch (tag) {
        case PktLogIn::TAG: {
            auto& pkt = packet.payload.emplace<PktLogIn>();
            read_from(buf, pkt);
            break;
        }
        case PktInputs::TAG: {
            auto& pkt = packet.payload.emplace<PktInputs>();
            read_from(buf, pkt);
            break;
        }
        case PktLogOut::TAG: {
            auto& pkt = packet.payload.emplace<PktLogOut>();
            read_from(buf, pkt);
            break;
        }
        default: 
            throw std::runtime_error("unknonwn pkt tag");
    }
    
    return packet;

    // possible solution to avoid the switch
    // TODO

    // if (!parse_payload(tag, buf, packet.payload)) {
    //     throw std::runtime_error("Unknown pkt tag");
    // }
    // return packet;
}

// template <typename Variant, size_t I = 0>
// bool parse_payload(uint8_t tag, ByteBuffer& buf, Variant& payload) {
//     if constexpr (I < std::variant_size_v<Variant>) {

//         using PktType = std::variant_alternative_t<I, Variant>;
//         if (tag == PktType::TAG) {
//             auto& pkt = payload.template emplace<PktType>();
//             read_from(buf, pkt);
//             return true;
//         }
//         return parse_payload<Variant, I + 1>(tag, buf, payload);
//     }
//     return false; 
// }