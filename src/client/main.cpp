#include <iostream>
#include <SFML/Graphics.hpp>

int main() {

    std::cout << "Hello, World! I'm Client" << std::endl;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Snake Client");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(30, 120, 50));
        window.display();
    }
    
    return 0;
}
