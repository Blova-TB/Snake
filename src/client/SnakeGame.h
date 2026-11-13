#pragma once

#include "dataObject/OtherPlayer.h"
#include "dataObject/UserPlayer.h"
#include "dataObject/PlayerPhy.h"

#include "SnakeNetwork.h"

#include <unordered_map>
#include <boost/asio.hpp>
#include <vector>

class SnakeGame
{
public:

    SnakeGame(SnakeNetwork &network, boost::asio::io_context &io_context);

private:
    UserPlayer userPlayer_;
    
    // joueursID -> Player
    std::unordered_map<int, OtherPlayer> players_;

    std::unordered_map<int,int> playersIdToPlayersPhyIndx_;
    std::vector<PlayerPhy> playersPhy_;
    
    std::chrono::milliseconds tickLength_;
    
    std::mutex eventsMutex_;
    std::vector<ServerToClientPacket> pendingEvents_;
    std::vector<ServerToClientPacket> curentEvents_;

    void start(boost::asio::io_context &io_context);

    void gameTick(boost::asio::steady_timer &timeLeft);

    void updatePhysics(double deltaTime);

    void inputsProcess();

    void networkEventsProcess();
    void collectEvents();
    void applyEvents();

    void handleSnapshot(const int playerId, const PktSnapshot& pkt);

    void safeAddEvent(const ServerToClientPacket& event);

};