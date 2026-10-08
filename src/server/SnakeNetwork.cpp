#include "SnakeNetwork.h"

#include <iostream>

using boost::asio::ip::udp;

SnakeNetwork::SnakeNetwork(boost::asio::io_context &io_context)
    : socket_(io_context)
{
    socket_.open(udp::v4());
    socket_.bind(udp::endpoint(udp::v4(), 13));
    start_receive();
}

void SnakeNetwork::start_receive()
{
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_),
        sender_endpoint_,
        [this](boost::system::error_code ec, size_t bytes_transferred)
        {
            handle_receive(ec, bytes_transferred);
            start_receive();
        });
}

void SnakeNetwork::handle_receive(const boost::system::error_code &error,
                                  std::size_t bytes_transferred)
{
    if (error)
    {
        std::cerr << "error : " << error.to_string() << std::endl;
        return;
    }

    if (bytes_transferred < sizeof(PacketHeader))
    {
        std::cerr << "error : wrong Header packet size" << std::endl;
        return;
    }

    PacketHeader *msgHeader = reinterpret_cast<PacketHeader *>(recv_buffer_.data());

    size_t totalSize = sizeof(PacketHeader) + msgHeader->size;
    if (bytes_transferred != totalSize)
    {
        std::cerr << "error : wrong packet size" << std::endl;
        return;
    }

    uint8_t *msgBody = recv_buffer_.data() + sizeof(PacketHeader);

    GameEvent event;
    
    if (auto optionalId = processPlayerId()) 
    {
        event.playerId = *optionalId;
    } 
    else 
    {
        std::cerr << "Erreur : Impossible de créer un ID.\n";
    }

    switch (msgHeader->type)
    {
    case MsgType::Connect:
    {
        MsgConnect *msg = reinterpret_cast<MsgConnect *>(msgBody);
        event.payload = *msg;
        break;
    }
    case MsgType::Disconnect:
    {
        MsgDisconnect *msg = reinterpret_cast<MsgDisconnect *>(msgBody);
        event.payload = *msg;
        break;
    }
    case MsgType::Input:
    {
        MsgInput *msg = reinterpret_cast<MsgInput *>(msgBody);
        event.payload = *msg;
    }
    }

    if (on_input_received)
    {
        on_input_received(event);
    }
}

void SnakeNetwork::handle_send(std::shared_ptr<std::string> /*message*/,
                               const boost::system::error_code & /*error*/,
                               std::size_t /*bytes_transferred*/)
{
}

std::optional<uint32_t> SnakeNetwork::processPlayerId()
{
    if (auto playerId = CoToPlayer_.find(sender_endpoint_); playerId != CoToPlayer_.end())
    {
        return playerId->second;
    }

    for (int i = 0; i < 5; i++)
    {
        uint32_t newId = std::rand();
        if (playersToCo_.find(newId) == playersToCo_.end())
        {
            playersToCo_.insert({newId, sender_endpoint_});
            CoToPlayer_.insert({sender_endpoint_, newId});
            return newId;
        }
    }

    return std::nullopt;
}
