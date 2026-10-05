#pragma once

#include <unordered_map>
#include <boost/asio.hpp>
#include "Player.h"
#include "PlayerPhy.h"
#include "../shared/PlayerInput.h"

class SnakeGame
{
public:
    const int MAX_PLAYERS = 100;

    SnakeGame();
    void start();

private:
    boost::asio::io_context io_;

    // joueursID -> Player
    std::unordered_map<int, Player> players_;
    std::vector<PlayerPhy> playersPhy_;
    
    
    std::mutex inputs_mutex_;
    std::vector<PlayerInput> curentInput_;
    std::vector<PlayerInput> pendingInputs_;

    std::chrono::milliseconds tickLength_;

    void gameTick(boost::asio::steady_timer &timeLeft);

    void updatePhysics(double deltaTime);

    void stateProcess();
    void calcState();
    void broadcastState();

    void inputProcess();
    void collectInput();
    void applyInput();
};