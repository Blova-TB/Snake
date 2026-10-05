#include "SnakeGame.h"

#include <iostream>
#include <boost/asio.hpp>


// void updateGame(boost::asio::steady_timer& t);


int main()
{
    std::cout << "Hello, World! I'm Server" << std::endl;
    SnakeGame snakeGame;
    snakeGame.start();


    // boost::asio::io_context io;
    // boost::asio::steady_timer t1(io, std::chrono::milliseconds(1000));

    // updateGame(t1);

    // io.run();
    return 0;
}

// void updateGame(boost::asio::steady_timer& t){
    
//     auto duration = boost::asio::chrono::steady_clock::now() - t.expiry() + std::chrono::milliseconds(1000);
//     double deltaTime = boost::asio::chrono::duration_cast<boost::asio::chrono::duration<double>>(duration).count();
//     t.expires_after(boost::asio::chrono::milliseconds(std::chrono::milliseconds(1000)));

//     std::cout << "Delta time: " << deltaTime << " seconds" << std::endl;

//     t.async_wait([&t](const boost::system::error_code& error) {
//         if (!error) {
//             updateGame(t);
//         }
//     });
// }
