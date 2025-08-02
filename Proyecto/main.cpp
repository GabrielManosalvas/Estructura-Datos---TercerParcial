#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <optional>

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "SFML Test");

    // Crear figuras de prueba - CORREGIDO para SFML 3.0
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(sf::Vector2f(100.f, 100.f));  // ← Cambio aquí

    sf::RectangleShape rectangle(sf::Vector2f(200, 100));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setPosition(sf::Vector2f(300.f, 200.f));  // ← Cambio aquí

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Blue);
        window.draw(circle);
        window.draw(rectangle);
        window.display();
    }

    return 0;
}
