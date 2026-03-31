#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "slots_game.hpp"

void centerTextInBox(sf::Text& text, const sf::RectangleShape& box);

void styleReel(sf::RectangleShape& reel, int symbol);

void drawSlotSymbol(sf::RenderWindow& window, int symbol, const sf::RectangleShape& inner, const sf::Font& font);

void drawPayoutTable(sf::RenderWindow& window, const sf::Font& font, const sf::RectangleShape& panel, int currentBet);

std::string resultMessageFromSpin(const SlotsResult& result, int currentBet, bool validBet);
