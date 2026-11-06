#pragma once

#include "Player.h"
#include "PlayerPhy.h"
#include "SnakeNetwork.h"

#include <unordered_map>
#include <boost/asio.hpp>
#include <vector>

class SnakeGame
{
public:
    const int MAX_PLAYERS = 100;

    SnakeGame(SnakeNetwork &network, boost::asio::io_context &io_context);

private:
    // joueursID -> Player
    std::unordered_map<int, Player> players_;

    std::unordered_map<int,int> playersIdToPlayersPhyIndx_;
    std::vector<PlayerPhy> playersPhy_;
    
    std::chrono::milliseconds tickLength_;
    
    std::mutex eventsMutex_;
    std::vector<GamePacket> pendingEvents_;
    std::vector<GamePacket> curentEvents_;

    void start(boost::asio::io_context &io_context);

    void gameTick(boost::asio::steady_timer &timeLeft);

    void updatePhysics(double deltaTime);

    void stateProcess();
    void calcState();
    void broadcastState();

    void eventsProcess();
    void collectEvents();
    void applyEvents();

    void handleLogIn(const int playerId, const PktLogIn& pkt);
    void handleLogOut(const int playerId, const PktLogOut& pkt);
    void handleInputs(const int playerId, const PktInputs& pkt);

    void safeAddEvent(const GamePacket& event);

};