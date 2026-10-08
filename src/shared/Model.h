#pragma once
#include <cstdint>
#include <variant>

enum class MsgType : uint8_t {
    Connect = 1,
    Disconnect = 2,
    Input = 3,
};

enum class InputFlags : uint8_t {
    None  = 0,
    Up    = 1 << 0,
    Down  = 1 << 1,
    Left  = 1 << 2,
    Right = 1 << 3,
    Action = 1 << 4
};


#pragma pack(push, 1)

// Header (3)
struct PacketHeader {
    MsgType type;
    uint16_t size;
};

// Connexion (16)
struct MsgConnect {
    char pseudo[16];
};

// Input (5)
struct MsgInput {
    uint32_t tickId;
    uint8_t keys;
};

// Leave 0
struct MsgDisconnect{
};

#pragma pack(pop)

using MessagePayload = std::variant<MsgConnect, MsgInput, MsgDisconnect>;

// 2. L'événement complet qui sera stocké dans la liste
struct GameEvent {
    int playerId;
    MessagePayload payload;
};