#include <SFML/Graphics.hpp>
#include "events.hpp"
#include "configs.hpp"
#include <InsectumVerlet.hpp>
#include "renderer.hpp"
#include <iostream>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({conf::WINDOW_X, conf::WINDOW_Y}), "CMake SFML Project");
    window.setFramerateLimit(conf::FRAME_RATE);

    Renderer renderer(window);
    verlet::Container container;

    container.setGravity(conf::GRAVITY);
    container.setUpdateRate(conf::FRAME_RATE);
    container.setConstraint(conf::WINDOW_X / 2, conf::WINDOW_Y/2, conf::WINDOW_Y/2);
    
    sf::Clock clock;

    while (window.isOpen())
    {
        handleEvents(window, container);

        if (container.getNumObjects() < 1000 && clock.getElapsedTime().asMilliseconds() > 50)
        {
            clock.restart();
            verlet::Object newObject(conf::WINDOW_X / 2, conf::WINDOW_Y / 4, 5);
            newObject.setVelocity(verlet::Vec2(1, 1));
            container.addObject(newObject);
        }

        window.clear(sf::Color::White);

        container.update();
        renderer.run(container);

        window.display();
    }
}
