#pragma once

#include <SFML/Graphics.hpp>
#include <InsectumVerlet.hpp>

class Renderer
{
    public:
        Renderer(sf::RenderTarget& renderTarget): renderTarget_(renderTarget) {}
        ~Renderer(){}

        void run(verlet::Container& container)
        {
            drawConstraint(container);

            sf::CircleShape circle(1.0f);
            circle.setOrigin(sf::Vector2f(1.0f, 1.0f));
            circle.setPointCount(32);
            circle.setFillColor(sf::Color::Cyan);

            for (verlet::Object o: container.getObjects())
            {
                sf::Vector2f position(o.getPosition().x, o.getPosition().y);
                sf::Vector2f scale(o.getRadius(), o.getRadius());
                circle.setPosition(position);
                circle.setScale(scale);
                renderTarget_.draw(circle);
            }
        }

    private:
        sf::RenderTarget& renderTarget_;

        void drawConstraint(verlet::Container& container)
        {
            sf::CircleShape circle(1.0f);
            circle.setOrigin(sf::Vector2f(1.0f, 1.0f));
            circle.setFillColor(sf::Color::Black);
            circle.setPointCount(32);
            circle.setScale(sf::Vector2f(container.getConstraintRadius(), container.getConstraintRadius()));
            circle.setPosition(sf::Vector2f(container.getConstraintCenter().x, container.getConstraintCenter().y));
            renderTarget_.draw(circle);
        }
};
