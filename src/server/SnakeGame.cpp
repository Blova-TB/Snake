#include "SnakeGame.h"

#include <iostream>
#include <boost/asio.hpp>

SnakeGame::SnakeGame(SnakeNetwork &network, boost::asio::io_context &io_context)
    : tickLength_(std::chrono::milliseconds(100))
{
    network.on_input_received = [this](const GameEvent& event)
    {
        safeAddEvent(event);
    };
    players_.reserve(MAX_PLAYERS);
    playersPhy_.reserve(MAX_PLAYERS);
    std::cout << "Hello, World! I'm Game" << std::endl;

    start(io_context);
}

void SnakeGame::start(boost::asio::io_context &io_context)
{
    std::cout << "gameTick() started" << std::endl;

    boost::asio::steady_timer t(io_context, std::chrono::milliseconds(0));
    t.async_wait([this, &t](const boost::system::error_code &error)
                 {
        if (!error) {
            this->gameTick(t);
        } });
}

void SnakeGame::gameTick(boost::asio::steady_timer &t)
{
    auto duration = boost::asio::chrono::steady_clock::now() - t.expiry() + tickLength_;
    double deltaTime = boost::asio::chrono::duration_cast<boost::asio::chrono::duration<double>>(duration).count();
    t.expires_after(boost::asio::chrono::milliseconds(tickLength_));

    std::cout << "New tick ___ Delta time: " << deltaTime << " seconds" << std::endl;

    updatePhysics(deltaTime);

    stateProcess();

    eventsProcess();

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

void SnakeGame::eventsProcess()
{
    collectEvents();
    applyEvents();
}

void SnakeGame::collectEvents()
{
    std::lock_guard<std::mutex> lock(eventsMutex_);
    std::swap(curentEvents_, pendingEvents_);
    pendingEvents_.clear();
}

void SnakeGame::safeAddEvent(const GameEvent& event)
{
    std::lock_guard<std::mutex> lock(eventsMutex_);
    pendingEvents_.push_back(event);
}

void SnakeGame::applyEvents()
{
    for (auto &input : curentEvents_)
    {
        std::cout << "Event !";
    }
}