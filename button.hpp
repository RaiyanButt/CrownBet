#pragma once

#include <SFML/Graphics.hpp>
#include <string>

/**
 * @brief Represents a clickable UI button.
 *
 * This class provides functionality for creating and managing a graphical
 * button using SFML. It supports rendering, text display, click detection,
 * and visual updates such as changing colors or labels.
 *
 * @author Jeffrey
 */
class Button 
{
public:
    /**
     * @brief Default constructor for Button.
     *
     * This constructor creates an uninitialized button. It is typically used
     * when the button will be assigned or configured later.
     *
     * @return None.
     *
     * @author Jeffrey
     */
    Button() = default;

    /**
     * @brief Constructs a Button with specified properties.
     *
     * This constructor initializes the button with size, position, color,
     * and text. It also sets up the font and character size used for rendering.
     *
     * @param size The size of the button.
     * @param position The position of the button on the screen.
     * @param fillColor The background color of the button.
     * @param textString The text displayed on the button.
     * @param font The font used for rendering the text.
     * @param charSize The size of the text characters.
     *
     * @return None.
     *
     * @author Jeffrey
     */
    Button(const sf::Vector2f& size,
        const sf::Vector2f& position,
        const sf::Color& fillColor,
        const std::string& textString,
        sf::Font& font,
        unsigned int charSize = 28);

    /**
     * @brief Sets the text displayed on the button.
     *
     * This function updates the button’s label and ensures that the text
     * remains centered within the button.
     *
     * @param textString The new text to display.
     *
     * @return None.
     *
     * @author Jeffrey
     */
    void setText(const std::string& textString);

    /**
     * @brief Sets the fill color of the button.
     *
     * This function updates the background color of the button, which is
     * often used to provide visual feedback such as hover effects.
     *
     * @param color The new fill color.
     *
     * @return None.
     *
     * @author Jeffrey
     */
    void setFillColor(const sf::Color& color);

    /**
     * @brief Checks if a point is inside the button.
     *
     * This function determines whether a given position (typically a mouse
     * click) lies within the bounds of the button.
     *
     * @param point The position to check.
     *
     * @return True if the point is inside the button, otherwise false.
     *
     * @author Jeffrey
     */
    bool contains(const sf::Vector2f& point) const;

    /**
     * @brief Draws the button on the screen.
     *
     * This function renders the button’s shape and text onto the provided
     * render window.
     *
     * @param window The render window used for drawing.
     *
     * @return None.
     *
     * @author Jeffrey
     */
    void draw(sf::RenderWindow& window) const;

private:
    /**
     * @brief Centers the text within the button.
     *
     * This helper function adjusts the text position so that it is aligned
     * in the center of the button's shape.
     *
     * @return None.
     *
     * @author Jeffrey
     */
    void centerText();

    sf::RectangleShape shape_;
    sf::Text text_;
};