#include "SnakeGame.h"

#include <iostream>
#include <boost/asio.hpp>

SnakeGame::SnakeGame()
    : io_(),
      tickLength_(std::chrono::milliseconds(100))
{
    players_.reserve(MAX_PLAYERS);
    playersPhy_.reserve(MAX_PLAYERS);
    std::cout << "Hello, World! I'm Game" << std::endl;
}

void SnakeGame::start()
{
    std::cout << "gameTick() started" << std::endl;

    boost::asio::steady_timer t(io_, std::chrono::milliseconds(0));
    t.async_wait([this, &t](const boost::system::error_code &error)
                 {
        if (!error) {
            this->gameTick(t);
        } });

    io_.run();
}

void SnakeGame::gameTick(boost::asio::steady_timer &t)
{
    auto duration = boost::asio::chrono::steady_clock::now() - t.expiry() + tickLength_;
    double deltaTime = boost::asio::chrono::duration_cast<boost::asio::chrono::duration<double>>(duration).count();
    t.expires_after(boost::asio::chrono::milliseconds(tickLength_));

    std::cout << "New tick ___ Delta time: " << deltaTime << " seconds" << std::endl;

    updatePhysics(deltaTime);

    stateProcess();

    inputProcess();

    t.async_wait([this, &t](const boost::system::error_code &error)
                 {
        if (!error) {
            gameTick(t);
        } });
}

void SnakeGame::updatePhysics(double deltaTime)
{
    for (PlayerPhy &plyr : playersPhy_)
    {
        plyr.setPosition(plyr.getPosition() + plyr.getVitesse() * deltaTime);
    }
}

void SnakeGame::stateProcess()
{
    calcState();
    broadcastState();
}

void SnakeGame::calcState()
{
    // TODO
}

void SnakeGame::broadcastState()
{
    // TODO
}

void SnakeGame::inputProcess()
{
    collectInput();
    applyInput();
}

void SnakeGame::collectInput()
{
    std::lock_guard<std::mutex> lock(inputs_mutex_);
    curentInput_ = pendingInputs_;
    pendingInputs_.clear();
}

void SnakeGame::applyInput()
{
    for(auto &input : curentInput_){
        // TODO 
    }
}