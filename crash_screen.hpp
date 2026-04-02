#pragma once

#include <SFML/Graphics.hpp>

#include "button.hpp"
#include "crash_game.hpp"
#include "rng.hpp"
#include "wallet.hpp"

/**
 * @brief Represents the crash game screen and its user interface.
 *
 * This class manages the visual layout and interaction logic for the
 * crash game screen. It handles user input, screen updates, hover effects,
 * and rendering of all interface elements such as buttons, labels,
 * the multiplier display, and the rocket graphic.
 *
 * @author Daniel
 */
class CrashScreen 
{
public:
    /**
     * @brief Constructs a CrashScreen object.
     *
     * This constructor initializes the crash screen and stores the font
     * used to render all text displayed on the interface.
     *
     * @param font The font used for text rendering on the crash screen.
     *
     * @author Daniel
     */
    explicit CrashScreen(sf::Font& font);

    /**
     * @brief Handles actions required before leaving the crash screen.
     *
     * This function ensures that any active round is properly resolved
     * before navigating away from the screen.
     *
     * @param wallet The player's wallet used for cash out or round cleanup.
     *
     * @return None.
     *
     * @author Daniel
     */
    void onNavigateAway(Wallet& wallet);

    /**
     * @brief Handles mouse click events on the crash screen.
     *
     * This function processes user input on interactive controls such as
     * bet buttons and the main launch or cash-out button.
     *
     * @param p The mouse click position.
     * @param wallet The player's wallet used for bets and payouts.
     * @param rng The random number generator used to start crash rounds.
     *
     * @return None.
     *
     * @author Daniel
     */
    void onMouseClick(const sf::Vector2f& p, Wallet& wallet, RNG& rng);

    /**
     * @brief Updates the crash screen state.
     *
     * This function refreshes the internal state of the crash screen,
     * including bet validation, round progression, and button text updates.
     *
     * @param wallet The player's wallet used to validate the selected bet.
     *
     * @return None.
     *
     * @author Daniel
     */
    void update(const Wallet& wallet);

    /**
     * @brief Updates hover effects for interactive elements.
     *
     * This function changes the appearance of screen buttons when the
     * mouse moves over them.
     *
     * @param mouse The current mouse position.
     *
     * @return None.
     *
     * @author Daniel
     */
    void updateHover(const sf::Vector2f& mouse);

    /**
     * @brief Draws the crash screen.
     *
     * This function renders all elements of the crash screen, including
     * labels, buttons, the rocket graphic, and game state information.
     *
     * @param window The render window used to display the screen.
     *
     * @return None.
     *
     * @author Daniel
     */
    void draw(sf::RenderWindow& window);

private:
    CrashGame game_;
    sf::RectangleShape panel_;
    sf::Text title_;
    sf::Text subLine1_;
    sf::Text subLine2_;
    sf::Text subLine3_;
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