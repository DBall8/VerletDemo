#include "events.hpp"

void handleEvents(sf::Window& window, verlet::Container& container)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        
        if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPress->scancode == sf::Keyboard::Scancode::Escape)
            {
                window.close();
            }
        }

        if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>())
        {
            container.addObject(verlet::Object(mouseClick->position.x, mouseClick->position.y, 20));
        }
    }
}