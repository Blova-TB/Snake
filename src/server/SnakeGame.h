#pragma once

#include <unordered_map>
#include <boost/asio.hpp>

#include "Player.h"
#include "PlayerPhy.h"
#include "SnakeNetwork.h"

class SnakeGame
{
public:
    const int MAX_PLAYERS = 100;

    SnakeGame(SnakeNetwork &network, boost::asio::io_context &io_context);

private:
    // joueursID -> Player
    std::unordered_map<int, Player> players_;
    std::vector<PlayerPhy> playersPhy_;
    
    std::chrono::milliseconds tickLength_;
    
    std::mutex eventsMutex_;
    std::vector<GameEvent> pendingEvents_;
    std::vector<GameEvent> curentEvents_;

    void start(boost::asio::io_context &io_context);

    void gameTick(boost::asio::steady_timer &timeLeft);

    void updatePhysics(double deltaTime);

    void stateProcess();
    void calcState();
    void broadcastState();

    void eventsProcess();
    void collectEvents();
    void applyEvents();

    void safeAddEvent(const GameEvent& event);
};