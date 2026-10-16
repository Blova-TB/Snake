#pragma once

#include <variant>
#include "../shared/Model.h"

using MessagePayload = std::variant<MsgConnect, MsgInput, MsgDisconnect>;

struct GameEvent {
    int senderId;
    MessagePayload payload;
};