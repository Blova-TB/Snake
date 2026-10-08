#pragma once

#include <boost/asio.hpp>
#include <unordered_map>
#include <optional>

#include "../shared/Model.h"

class SnakeNetwork
{
public:
    SnakeNetwork(boost::asio::io_context &io_context);

    std::function<void(const GameEvent&)> on_input_received;

private:
    void start_receive();

    void handle_receive(const boost::system::error_code &error,
                        std::size_t size);

    void handle_send(std::shared_ptr<std::string> /*message*/,
                     const boost::system::error_code & /*error*/,
                     std::size_t /*bytes_transferred*/);

    std::optional<uint32_t> processPlayerId();


    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint sender_endpoint_;
    std::array<uint8_t, 1024> recv_buffer_;

    // player id -> 
    std::unordered_map<uint32_t, boost::asio::ip::udp::endpoint> playersToCo_;
    std::unordered_map<boost::asio::ip::udp::endpoint,uint32_t> CoToPlayer_;
};
