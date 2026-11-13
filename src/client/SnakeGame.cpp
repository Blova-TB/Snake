#include "SnakeGame.h"
#include "../shared/SharedProtocol.h"
#include "../shared/Const.h"

#include <iostream>
#include <variant>
#include <boost/asio.hpp>

SnakeGame::SnakeGame(SnakeNetwork &network, boost::asio::io_context &io_context)
    : tickLength_(std::chrono::milliseconds(CLIENT_MIN_TICK_LENGTH)),
      userPlayer_(0, "PlaceHolder")
{
    network.on_pkt_received = [this](const ServerToClientPacket &event)
    {
        safeAddEvent(event);
    };
    players_.reserve(MAX_PLAYERS);
    playersIdToPlayersPhyIndx_.reserve(MAX_PLAYERS);
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

    inputsProcess();

    networkEventsProcess();

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

void SnakeGame::inputsProcess()
{
    //TODO recup et envoyer au serv
}

void SnakeGame::networkEventsProcess()
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

void SnakeGame::safeAddEvent(const ServerToClientPacket &event)
{
    std::lock_guard<std::mutex> lock(eventsMutex_);
    pendingEvents_.push_back(event);
}

void SnakeGame::applyEvents()
{
    for (auto &event : curentEvents_)
    {
        std::visit([this, &event](const auto &pkt)
                   {
            using PktType = std::decay_t<decltype(pkt)>;

            if constexpr (std::is_same_v<PktType, PktSnapshot>)
            {
                handleSnapshot(event.playerId, pkt);
            }
            else
            {
                std::cerr << "Unknown event.payload" << std::endl;
            } }, event.payload);

        std::cout << "Event !" << std::endl;
    }
}

void SnakeGame::handleSnapshot(const int playerId, const PktSnapshot& pkt)
{
    // TODO
    return;
}
