#include <SFML/Graphics.hpp>
#include "events.hpp"
#include "configs.hpp"
#include <InsectumVerlet.hpp>
#include "renderer.hpp"
#include <iostream>
#include "timing.hpp"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({conf::WINDOW_X, conf::WINDOW_Y}), "CMake SFML Project");
    window.setFramerateLimit(conf::FRAME_RATE);

    Renderer renderer(window);
    verlet::Container container;

    container.setGravity(conf::GRAVITY);
    container.setUpdateRate(conf::FRAME_RATE);
    container.setPhysicsSubsteps(conf::QUALITY);
    // container.setCircleConstraint(conf::WINDOW_X / 2.0f, conf::WINDOW_Y / 2.0f, conf::WINDOW_Y / 2.0f);
    container.setRectConstraint(conf::WINDOW_X / 2.0f, conf::WINDOW_Y / 2.0f, conf::WINDOW_X, conf::WINDOW_Y);
    
    sf::Clock clock;
    sf::Clock startupClock;
    float timeAvg = 0;
    bool addObj = true;

    int numStreams = 4;

    while (window.isOpen())
    {
        auto start = std::chrono::high_resolution_clock::now();
        handleEvents(window, container);

        if (addObj && container.getNumObjects() < 400 && clock.getElapsedTime().asMilliseconds() > 50)
        {
            clock.restart();

            for (int i=0; i<numStreams; i++)
            {
                verlet::Object newObject(conf::WINDOW_X / 2 + (i*11), conf::WINDOW_Y / 4, 5);
                newObject.setVelocity(verlet::Vec2(1, 1));
                container.addObject(newObject);
            }
        }

        // startTiming();
        container.update();
        // std::cout << "c: " << endTiming() << std::endl;

        // startTiming();
        window.clear(sf::Color::White);
        renderer.run(container);
        renderer.drawFrames(16);

        window.display();
        // std::cout << "d: " << endTiming() << std::endl;

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        timeAvg = (timeAvg * 0.9f) + duration.count() * 0.1f;
        if (addObj && startupClock.getElapsedTime().asMilliseconds() > 3000 && timeAvg > 32000)
        {
            std::cout << "STOPPED AT " << container.getNumObjects() << "\n";
            addObj = false;
            window.close();
        }
        // std::cout << timeAvg << std::endl;
    }
}
