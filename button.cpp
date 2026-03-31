#include "button.hpp"

Button::Button(const sf::Vector2f& size,
    const sf::Vector2f& position,
    const sf::Color& fillColor,
    const std::string& textString,
    sf::Font& font,
    unsigned int charSize)
{
    shape_.setSize(size);
    shape_.setPosition(position);
    shape_.setFillColor(fillColor);
    shape_.setOutlineThickness(3.f);
    shape_.setOutlineColor(sf::Color::Black);

    text_.setFont(font);
    text_.setString(textString);
    text_.setCharacterSize(charSize);
    text_.setFillColor(sf::Color::Black);
    centerText();
}

void Button::setText(const std::string& textString)
{
    text_.setString(textString);
    centerText();
}

void Button::setFillColor(const sf::Color& color)
{
    shape_.setFillColor(color);
}

bool Button::contains(const sf::Vector2f& point) const
{
    return shape_.getGlobalBounds().contains(point);
}

void Button::draw(sf::RenderWindow& window) const
{
    window.draw(shape_);
    window.draw(text_);
}

void Button::centerText()
{
    const sf::FloatRect textBounds = text_.getLocalBounds();
    const sf::FloatRect shapeBounds = shape_.getGlobalBounds();

    text_.setOrigin(
        textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height / 2.f);

    text_.setPosition(
        shapeBounds.left + shapeBounds.width / 2.f,
        shapeBounds.top + shapeBounds.height / 2.f);
}
