#include "SnakeGame.h"
#include "../shared/ModelServer.h"

#include <iostream>
#include <variant>
#include <boost/asio.hpp>

SnakeGame::SnakeGame(SnakeNetwork &network, boost::asio::io_context &io_context)
    : tickLength_(std::chrono::milliseconds(100))
{
    network.on_pkt_received = [this](const GamePacket &event)
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
    for (auto ply : playersPhy_)
    {
        std::cout << ply.getPosition().toString() << std::endl;
    }
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

void SnakeGame::safeAddEvent(const GamePacket &event)
{
    std::lock_guard<std::mutex> lock(eventsMutex_);
    pendingEvents_.push_back(event);
}

void SnakeGame::applyEvents()
{
    for (auto &event : curentEvents_)
    {
        event.playerId;

        std::visit([this, &event](const auto &pkt)
                   {
            using PktType = std::decay_t<decltype(pkt)>;

            if constexpr (std::is_same_v<PktType, PktLogIn>)
            {
                handleLogIn(event.playerId, pkt);
            }
            else if constexpr (std::is_same_v<PktType, PktLogOut>)
            {
                handleLogOut(event.playerId, pkt);
            }
            else if constexpr (std::is_same_v<PktType, PktInputs>)
            {
                handleInputs(event.playerId, pkt);
            }
            else
            {
                std::cerr << "Unknown event.payload" << std::endl;
            } }, event.payload);

        std::cout << "Event !" << std::endl;
    }
}

void SnakeGame::handleLogIn(const int playerId, const PktLogIn &pkt)
{
    players_.insert(std::pair(
        playerId,
        Player(playerId, pkt.pseudo)));

    playersIdToPlayersPhyIndx_.insert(std::pair(playerId,playersPhy_.size()));
    
    playersPhy_.push_back(
        PlayerPhy(playerId)
    );
}

void SnakeGame::handleLogOut(const int playerId, const PktLogOut &pkt)
{
    players_.erase(playerId);
    int index_to_remove = playersIdToPlayersPhyIndx_[playerId];
    if (index_to_remove < playersPhy_.size() - 1) {
        playersPhy_[index_to_remove] = playersPhy_.back();
    }
    playersPhy_.pop_back();

    playersIdToPlayersPhyIndx_.erase(playerId);
    playersIdToPlayersPhyIndx_.insert_or_assign(playersPhy_[index_to_remove].getId(),index_to_remove);
}

void SnakeGame::handleInputs(const int playerId, const PktInputs &pkt)
{
    int index = playersIdToPlayersPhyIndx_[playerId];
    uint8_t input = (pkt.inputs[0].action_mask);

    Vector2D newSpeed(0,0);

    if(!inputContain(input, InputFlags::None)){
        if (inputContain(input, InputFlags::Up)){
            newSpeed.y -= 1; 
        }
        if (inputContain(input, InputFlags::Down)){
            newSpeed.y += 1; 
        }
        if (inputContain(input, InputFlags::Right)){
            newSpeed.x += 1; 
        }
        if (inputContain(input, InputFlags::Left)){
            newSpeed.x -= 1; 
        }
    }
    
    newSpeed.norm();
    playersPhy_[index].setVitesse(newSpeed);
}

void SnakeGame::createNewPlayer()
{
}