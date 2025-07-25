#include <SFML/Graphics.hpp>
#include "events.hpp"
#include "configs.hpp"
#include <InsectumVerlet.hpp>
#include "renderer.hpp"
#include <iostream>
#include "timing.hpp"

#define MAX_OBJS            10000
#define KILL_AT_FRAME_LIMIT 1
#define SHOW_FRAMES         1
int main()
{
    initTiming();
    auto window = sf::RenderWindow(sf::VideoMode({conf::WINDOW_X, conf::WINDOW_Y}), "CMake SFML Project");
    window.setFramerateLimit(conf::FRAME_RATE);

    Renderer renderer(window, conf::WINDOW_X, conf::WINDOW_Y);
    verlet::Container container;

    container.setGravity(conf::GRAVITY);
    container.setUpdateRate(conf::FRAME_RATE);
    container.setPhysicsSubsteps(conf::QUALITY);
    // container.setCircleConstraint(conf::WINDOW_X / 2.0f, conf::WINDOW_Y / 2.0f, conf::WINDOW_Y / 2.0f);
    container.setRectConstraint(conf::WINDOW_X / 2.0f, conf::WINDOW_Y / 2.0f, conf::WINDOW_X, conf::WINDOW_Y);

    // verlet::Object obstacle(conf::WINDOW_X * (0.60), conf::WINDOW_Y * (0.75), 40);
    // container.addFixedObject(obstacle);
    
    sf::Clock startupClock;
    sf::Clock frameClock;
    int frameCount = 0;
    bool addObj = true;

    int addObjCount = 0;
    int numStreams = 10;
    int fps = 0;

    while (window.isOpen())
    {
        auto start = std::chrono::high_resolution_clock::now();
        handleEvents(window, container);

        frameCount++;
        addObjCount++;

        if (addObj && container.getNumObjects() < MAX_OBJS && addObjCount > 3)
        {
            for (int i=0; i<numStreams; i++)
            {
                verlet::Object newObject(conf::WINDOW_X / 2 + (i*11), conf::WINDOW_Y / 4, 5);
                newObject.setVelocity(verlet::Vec2(400, 400));
                container.addObject(newObject);
            }

            addObjCount = 0;
        }

        container.update();

        window.clear(sf::Color::White);
        renderer.run(container);
    #if SHOW_FRAMES
        renderer.drawFrames(fps);
    #endif
        window.display();

        if (frameClock.getElapsedTime().asMilliseconds() >= 1000)
        {
            fps = frameCount;
            frameCount = 0;
            frameClock.restart();
        }


#if KILL_AT_FRAME_LIMIT

        if (addObj && startupClock.getElapsedTime().asMilliseconds() > 3000 && fps < 58)
        {
            std::cout << "STOPPED AT " << container.getNumObjects() << "\n";
            addObj = false;
            // window.close();
        }
#endif
        // std::cout << timeAvg << std::endl;
    }
}
