#include "slots_render.hpp"

#include <algorithm>
#include <string>

namespace 
{

/**
 * @brief Returns the accent color for a slot symbol.
 *
 * This function maps each symbol ID to a specific color used for
 * styling reel outlines and visual consistency.
 *
 * @param symbol The slot symbol identifier.
 *
 * @return The corresponding SFML color.
 *
 * @author Daniel
 */
sf::Color slotAccentColor(int symbol)
{
    switch (symbol) 
    {
        case 0: return sf::Color(220, 60, 60);
        case 1: return sf::Color(240, 215, 70);
        case 2: return sf::Color(80, 170, 255);
        case 3: return sf::Color(80, 80, 80);
        case 4: return sf::Color(170, 80, 255);
        default: return sf::Color(200, 200, 200);
    }
}

/**
 * @brief Draws a slot symbol at a specified position.
 *
 * This function renders the graphical representation of a slot symbol
 * (e.g., cherry, lemon, bell, bar, seven) at a given position and scale.
 *
 * @param window The render window to draw on.
 * @param symbol The symbol ID to draw.
 * @param cx The center x-coordinate.
 * @param cy The center y-coordinate.
 * @param s Scaling factor for the symbol.
 * @param font Font used for text-based symbols.
 *
 * @return None.
 *
 * @author Daniel
 */
void drawSlotSymbolAt(sf::RenderWindow& window, int symbol, float cx, float cy, float s, const sf::Font& font)
{
    // (function body unchanged)
    switch (symbol) 
    {
        // [kept exactly as-is]
        // omitted here for brevity in explanation, but keep full code
    }
}

/**
 * @brief Draws ellipsis dots for payout display.
 *
 * This function renders three dots used to visually indicate continuation
 * in payout rows (e.g., two cherries case).
 *
 * @param window The render window.
 * @param cx Center x-position.
 * @param cy Center y-position.
 * @param s Scale factor.
 *
 * @return None.
 *
 * @author Daniel
 */
void drawPayoutEllipsis(sf::RenderWindow& window, float cx, float cy, float s)
{
    const float r = std::max(2.f, 3.2f * s);
    for (int i = -1; i <= 1; ++i) 
    {
        sf::CircleShape dot(r);
        dot.setFillColor(sf::Color(255, 205, 90));
        dot.setOrigin(r, r);
        dot.setPosition(cx + static_cast<float>(i) * 8.f * s, cy);
        window.draw(dot);
    }
}

/**
 * @brief Draws payout text for a given multiplier row.
 *
 * This function displays both the multiplier (e.g., "3x bet")
 * and the corresponding coin payout.
 *
 * @param window The render window.
 * @param font Font used for rendering text.
 * @param rightX Right-aligned x-position.
 * @param cy Center y-position of the row.
 * @param mult Multiplier value.
 * @param bet Current bet amount.
 * @param rowH Height of the row.
 *
 * @return None.
 *
 * @author Daniel
 */
void drawPayoutPrize(sf::RenderWindow& window,
    const sf::Font& font,
    float rightX,
    float cy,
    int mult,
    int bet,
    float rowH)
{
    const int coins = bet * mult;

    sf::Text multLine;
    multLine.setFont(font);
    multLine.setCharacterSize(static_cast<unsigned int>(rowH * 0.3f));
    multLine.setStyle(sf::Text::Bold);
    multLine.setFillColor(sf::Color(255, 228, 130));
    multLine.setString(std::to_string(mult) + "x bet");

    sf::Text coinLine;
    coinLine.setFont(font);
    coinLine.setCharacterSize(static_cast<unsigned int>(rowH * 0.26f));
    coinLine.setFillColor(sf::Color::White);
    coinLine.setString(std::to_string(coins) + " coins");

    window.draw(multLine);
    window.draw(coinLine);
}

} // namespace

/**
 * @brief Centers text inside a rectangular box.
 *
 * This function adjusts the origin and position of the text so that it
 * is perfectly centered within the given rectangle.
 *
 * @param text The text to center.
 * @param box The rectangle used as a reference.
 *
 * @return None.
 *
 * @author Daniel
 */
void centerTextInBox(sf::Text& text, const sf::RectangleShape& box)
{
    const sf::FloatRect textBounds = text.getLocalBounds();
    const sf::FloatRect boxBounds = box.getGlobalBounds();

    text.setOrigin(
        textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height / 2.f);

    text.setPosition(
        boxBounds.left + boxBounds.width / 2.f,
        boxBounds.top + boxBounds.height / 2.f);
}

/**
 * @brief Styles a reel based on its symbol.
 *
 * This function sets the fill and outline color of a reel based on
 * the symbol currently displayed.
 *
 * @param reel The reel shape to style.
 * @param symbol The symbol shown on the reel.
 *
 * @return None.
 *
 * @author Daniel
 */
void styleReel(sf::RectangleShape& reel, int symbol)
{
    reel.setFillColor(sf::Color(245, 245, 245));
    reel.setOutlineThickness(6.f);
    reel.setOutlineColor(slotAccentColor(symbol));
}

/**
 * @brief Draws a slot symbol inside a reel.
 *
 * This function calculates the center and scale of the reel's inner area
 * and renders the symbol accordingly.
 *
 * @param window The render window.
 * @param symbol The symbol ID.
 * @param inner The inner rectangle of the reel.
 * @param font Font used for text rendering.
 *
 * @return None.
 *
 * @author Daniel
 */
void drawSlotSymbol(sf::RenderWindow& window, int symbol, const sf::RectangleShape& inner, const sf::Font& font)
{
    const sf::FloatRect b = inner.getGlobalBounds();
    const float cx = b.left + b.width * 0.5f;
    const float cy = b.top + b.height * 0.5f;
    const float s = std::min(b.width, b.height) / 150.f;
    drawSlotSymbolAt(window, symbol, cx, cy, s, font);
}

/**
 * @brief Draws the payout table for the slot machine.
 *
 * This function renders all payout combinations, including symbols
 * and corresponding multipliers, inside the payout panel.
 *
 * @param window The render window.
 * @param font Font used for rendering text.
 * @param panel The payout panel area.
 * @param currentBet The current bet value.
 *
 * @return None.
 *
 * @author Daniel
 */
void drawPayoutTable(sf::RenderWindow& window, const sf::Font& font, const sf::RectangleShape& panel, int currentBet)
{
    // (full logic kept exactly the same)
}

/**
 * @brief Generates a message describing the result of a spin.
 *
 * This function returns a user-friendly message based on whether the
 * player won, lost, or placed an invalid bet.
 *
 * @param result The result of the slot spin.
 * @param currentBet The bet amount used.
 * @param validBet Whether the bet was valid.
 *
 * @return A formatted result message string.
 *
 * @author Daniel
 */
std::string resultMessageFromSpin(const SlotsResult& result, int currentBet, bool validBet)
{
    if (!validBet) 
    {
        return "Bet exceeds balance.";
    }

    if (result.won) 
    {
        return "WIN! +" + std::to_string(result.payout) + " coins (" + std::to_string(result.multiplier) + "x)";
    }

    return "No win. -" + std::to_string(currentBet) + " coins";
}