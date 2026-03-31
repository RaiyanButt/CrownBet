#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button() = default;

    Button(const sf::Vector2f& size,
        const sf::Vector2f& position,
        const sf::Color& fillColor,
        const std::string& textString,
        sf::Font& font,
        unsigned int charSize = 28);

    void setText(const std::string& textString);
    void setFillColor(const sf::Color& color);

    bool contains(const sf::Vector2f& point) const;
    void draw(sf::RenderWindow& window) const;

private:
    void centerText();

    sf::RectangleShape shape_;
    sf::Text text_;
};
