#include "SnakeGame.h"
#include "SnakeNetwork.h"

#include <iostream>
#include <boost/asio.hpp>


// void updateGame(boost::asio::steady_timer& t);


int main()
{
    std::cout << "Hello, World! I'm Server" << std::endl;
    boost::asio::io_context io_context;
    SnakeNetwork network(io_context);
    SnakeGame snakeGame(network,io_context);

    io_context.run();
    return 0;
}