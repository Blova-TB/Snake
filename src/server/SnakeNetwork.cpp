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

void SnakeNetwork::handle_receive(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (error)
    {
        std::cerr << "error : " << error.to_string() << std::endl;
        return;
    }

    try
    {
        GamePacket packet = read_packet(recv_buffer_.data(), bytes_transferred);

        if (auto optionalId = processPlayerId())
        {
            if (packet.playerId = 0)
            {
                packet.playerId = *optionalId;
            }
            else
            {
                if (packet.playerId != *optionalId)
                {
                    throw std::runtime_error("unknonwn player id");
                }
            }
        }
        else
        {
            std::cerr << "Err : ID not found and can't create.\n";
        }

        if (on_input_received)
        {
            on_input_received(packet);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Err : " << e.what() << std::endl;
    }
}

void SnakeNetwork::handle_send(std::shared_ptr<std::string> /*message*/,
                               const boost::system::error_code & /*error*/,
                               std::size_t /*bytes_transferred*/)
{
    //TODO
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
