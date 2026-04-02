#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "slots_game.hpp"

/**
 * @brief Centers text within a rectangular box.
 *
 * This function adjusts the origin and position of the given text so that
 * it appears centered inside the specified rectangle.
 *
 * @param text The text object to be centered.
 * @param box The rectangle used as the bounding area.
 *
 * @return None.
 *
 * @author Daniel
 */
void centerTextInBox(sf::Text& text, const sf::RectangleShape& box);

/**
 * @brief Applies styling to a slot reel based on its symbol.
 *
 * This function sets visual properties such as fill and outline color
 * depending on the symbol currently displayed on the reel.
 *
 * @param reel The reel shape to style.
 * @param symbol The symbol identifier for the reel.
 *
 * @return None.
 *
 * @author Daniel
 */
void styleReel(sf::RectangleShape& reel, int symbol);

/**
 * @brief Draws a slot symbol inside a given rectangle.
 *
 * This function renders the visual representation of a slot symbol
 * centered within the provided rectangle.
 *
 * @param window The render window used for drawing.
 * @param symbol The symbol identifier to draw.
 * @param inner The rectangle defining the drawing area.
 * @param font The font used for text-based symbols.
 *
 * @return None.
 *
 * @author Daniel
 */
void drawSlotSymbol(sf::RenderWindow& window, int symbol, const sf::RectangleShape& inner, const sf::Font& font);

/**
 * @brief Draws the payout table for the slot machine.
 *
 * This function displays all possible winning combinations and their
 * corresponding payouts based on the current bet.
 *
 * @param window The render window used for drawing.
 * @param font The font used for rendering text.
 * @param panel The rectangle defining the payout table area.
 * @param currentBet The current bet amount used for payout calculations.
 *
 * @return None.
 *
 * @author Daniel
 */
void drawPayoutTable(sf::RenderWindow& window, const sf::Font& font, const sf::RectangleShape& panel, int currentBet);

/**
 * @brief Generates a result message for a slot spin.
 *
 * This function returns a formatted string describing the outcome
 * of a spin, including winnings or losses.
 *
 * @param result The result of the slot spin.
 * @param currentBet The bet amount used for the spin.
 * @param validBet Indicates whether the bet was valid.
 *
 * @return A string describing the spin result.
 *
 * @author Daniel
 */
std::string resultMessageFromSpin(const SlotsResult& result, int currentBet, bool validBet);