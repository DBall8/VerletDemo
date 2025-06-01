#pragma once

#include <SFML/Graphics.hpp>
#include <InsectumVerlet.hpp>

class Renderer
{
    public:
        Renderer(sf::RenderTarget& renderTarget): renderTarget_(renderTarget)
        {
            if (!font.openFromFile("arial.ttf"))
            {
                std::cout << "Failed to load fonts\n";
            }
        }

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

        void drawFrames(int frames)
        {
            sf::Text text(font);
            text.setString("test");
            text.setFillColor(sf::Color::White);
            renderTarget_.draw(text);
        }

    private:
        sf::RenderTarget& renderTarget_;
        sf::Font font;

        void drawConstraint(verlet::Container& container)
        {
            verlet::Constraint constraint = container.getConstraint();
            if (constraint.isCircle)
            {
                float radiusConstraint = constraint.width / 2.0f;
                sf::CircleShape circle(1.0f);
                circle.setOrigin(sf::Vector2f(1.0f, 1.0f));
                circle.setFillColor(sf::Color::Black);
                circle.setPointCount(32);
                circle.setScale(sf::Vector2f(radiusConstraint, radiusConstraint));
                circle.setPosition(sf::Vector2f(constraint.position.x, constraint.position.y));
                renderTarget_.draw(circle);
            }
            else
            {
                sf::RectangleShape rectangle(sf::Vector2(constraint.width, constraint.height));
                rectangle.setOrigin(sf::Vector2f(0,0));
                rectangle.setFillColor(sf::Color::Black);
                renderTarget_.draw(rectangle);
            }
        }
};
