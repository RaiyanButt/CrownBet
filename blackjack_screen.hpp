#pragma once

#include <SFML/Graphics.hpp>

#include "blackjack_game.hpp"
#include "button.hpp"
#include "rng.hpp"
#include "wallet.hpp"

class BlackjackScreen {
public:
    explicit BlackjackScreen(sf::Font& font);

    void onNavigateAway(Wallet& wallet, RNG& rng);
    void onMouseClick(const sf::Vector2f& p, Wallet& wallet, RNG& rng);
    void update(const Wallet& wallet);
    void updateHover(const sf::Vector2f& mouse);
    void draw(sf::RenderWindow& window);

private:
    sf::Font& font_;
    BlackjackGame game_;
    sf::RectangleShape panel_;
    sf::Text title_;
    sf::Text payRulesTitle_;
    sf::Text payRulesLine1_;
    sf::Text payRulesLine2_;
    sf::Text payRulesLine3_;
    sf::Text cardRulesTitle_;
    sf::Text cardRulesLine1_;
    sf::Text cardRulesLine2_;
    sf::Text cardRulesLine3_;
    sf::Text playerLabel_;
    sf::Text playerTotal_;
    sf::Text status_;
    sf::Text betLabel_;
    sf::RectangleShape betBox_;
    Button betMinus_;
    Button betPlus_;
    Button dealBtn_;
    Button hitBtn_;
    Button standBtn_;
    Button newHandBtn_;
};
