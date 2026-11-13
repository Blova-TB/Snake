#pragma once

#include "SerializationTemplate.h"

#include <variant>

template <typename PayloadVariant>
struct GamePacket
{
    int playerId;
    PayloadVariant payload;
};

template <typename Variant, size_t I = 0>
bool parse_payload(uint8_t tag, ByteBuffer& buf, Variant& payload) {
    if constexpr (I < std::variant_size_v<Variant>) {

        using PktType = std::variant_alternative_t<I, Variant>;
        if (tag == PktType::TAG) {
            auto& pkt = payload.template emplace<PktType>();
            read_from(buf, pkt);
            return true;
        }
        return parse_payload<Variant, I + 1>(tag, buf, payload);
    }
    return false;
}

template <typename PayloadVariant>
void inline write_to(std::vector<uint8_t> &buf, const GamePacket<PayloadVariant> &packet)
{

    write_to(buf, packet.playerId);

    std::visit([&buf](const auto &pkt)
               {
        using PktType = std::decay_t<decltype(pkt)>; 
        write_to(buf, PktType::TAG); 
        write_to(buf, pkt); }, packet.payload);
}

template<typename PayloadVariant>
GamePacket<PayloadVariant> inline read_packet(const uint8_t *raw_data, size_t length)
{
    ByteBuffer buf{raw_data, length, 0};
    GamePacket packet;

    read_from(buf, packet.playerId);

    uint8_t tag;
    read_from(buf, tag);

    if (!parse_payload(tag, buf, packet.payload)) {
        throw std::runtime_error("Unknown pkt tag");
    }
    return packet;
}

