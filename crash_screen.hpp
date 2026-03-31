#pragma once

#include <SFML/Graphics.hpp>

#include "button.hpp"
#include "crash_game.hpp"
#include "rng.hpp"
#include "wallet.hpp"

class CrashScreen {
public:
    explicit CrashScreen(sf::Font& font);

    void onNavigateAway(Wallet& wallet);
    void onMouseClick(const sf::Vector2f& p, Wallet& wallet, RNG& rng);
    void update(const Wallet& wallet);
    void updateHover(const sf::Vector2f& mouse);
    void draw(sf::RenderWindow& window);

private:
    CrashGame game_;
    sf::RectangleShape panel_;
    sf::Text title_;
    sf::Text subLine1_;
    sf::Text subLine2_;
    sf::Text mult_;
    sf::Text status_;
    sf::Text betLabel_;
    sf::RectangleShape betBox_;
    sf::ConvexShape rocket_;
    Button betMinus_;
    Button betPlus_;
    Button mainBtn_;
    sf::Clock tickClock_;
};
