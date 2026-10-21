#pragma once

#include "ModelServer.h"
#include "SerializationTemplate.h"

#include <variant>

using MessagePayload = std::variant<PktInputs, PktLogIn, PktLogOut>;

struct GamePacket
{
    int playerId;
    MessagePayload payload;
};

void write_to(std::vector<uint8_t> &buf, const GamePacket &packet)
{

    write_to(buf, packet.playerId);

    std::visit([&buf](const auto &pkt)
               {
        using PktType = std::decay_t<decltype(pkt)>; 
        write_to(buf, PktType::TAG); 
        write_to(buf, pkt); }, packet.payload);
}

GamePacket read_packet(const uint8_t *raw_data, size_t length)
{
    ByteBuffer buf{raw_data, length, 0};
    GamePacket packet;

    read_from(buf, packet.playerId);

    uint8_t tag;
    read_from(buf, tag);

    switch (tag)
    {
    case PktLogIn::TAG:
    {
        auto &pkt = packet.payload.emplace<PktLogIn>();
        read_from(buf, pkt);
        break;
    }
    case PktInputs::TAG:
    {
        auto &pkt = packet.payload.emplace<PktInputs>();
        read_from(buf, pkt);
        break;
    }
    case PktLogOut::TAG:
    {
        auto &pkt = packet.payload.emplace<PktLogOut>();
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