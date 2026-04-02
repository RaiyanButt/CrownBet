#include "button.hpp"

/**
 * @brief Constructs a Button object.
 *
 * This constructor initializes a button with a specified size, position,
 * color, and text label. It also sets up the button's visual appearance,
 * including its outline and text formatting, and ensures the text is
 * centered within the button.
 *
 * @param size The size of the button (width and height).
 * @param position The position of the button on the screen.
 * @param fillColor The background color `of the button.
 * @param textString The text displayed on the button.
 * @param font The font used to render the button text.
 * @param charSize The size of the text characters.
 *
 * @return None.
 *
 * @author Jeffrey
 */
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

/**
 * @brief Sets the button's displayed text.
 *
 * This function updates the text string shown on the button and ensures
 * that the new text remains centered within the button.
 *
 * @param textString The new text to display on the button.
 *
 * @return None.
 *
 * @author Jeffrey
 */
void Button::setText(const std::string& textString)
{
    text_.setString(textString);
    centerText();
}

/**
 * @brief Sets the button's fill color.
 *
 * This function changes the background color of the button, typically used
 * for visual feedback such as hover or click effects.
 *
 * @param color The new fill color of the button.
 *
 * @return None.
 *
 * @author Jeffrey
 */
void Button::setFillColor(const sf::Color& color)
{
    shape_.setFillColor(color);
}

/**
 * @brief Checks whether a point is inside the button.
 *
 * This function determines if a given position (such as a mouse click)
 * lies within the bounds of the button.
 *
 * @param point The position to check.
 *
 * @return True if the point is inside the button, otherwise false.
 *
 * @author Jeffrey
 */
bool Button::contains(const sf::Vector2f& point) const
{
    return shape_.getGlobalBounds().contains(point);
}

/**
 * @brief Draws the button to the window.
 *
 * This function renders both the button's shape and its text onto the
 * provided render window.
 *
 * @param window The render window used for drawing.
 *
 * @return None.
 *
 * @author Jeffrey
 */
void Button::draw(sf::RenderWindow& window) const
{
    window.draw(shape_);
    window.draw(text_);
}

/**
 * @brief Centers the text within the button.
 *
 * This helper function calculates the bounds of the text and button,
 * then adjusts the text's position so it appears centered inside
 * the button shape.
 *
 * @return None.
 *
 * @author Jeffrey
 */
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