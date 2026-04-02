#pragma once

#include <SFML/Graphics.hpp>

#include "blackjack_game.hpp"
#include "button.hpp"
#include "rng.hpp"
#include "wallet.hpp"

/**
 * @brief Represents the blackjack game screen and its user interface.
 *
 * This class manages the visual presentation and user interaction for the
 * blackjack portion of the program. It handles button input, updates the
 * displayed game state, responds to navigation events, and renders all
 * screen elements such as labels, cards, and controls.
 *
 * @author Alexander
 */
class BlackjackScreen 
{
public:
    /**
     * @brief Constructs a BlackjackScreen object.
     *
     * This constructor initializes the blackjack screen and stores the font
     * used to render all text-based interface elements.
     *
     * @param font The font used for text displayed on the blackjack screen.
     *
     * @author Alexander
     */
    explicit BlackjackScreen(sf::Font& font);

    /**
     * @brief Handles actions required before leaving the blackjack screen.
     *
     * This function ensures that any active round is properly resolved before
     * navigating away from the screen. It also resets the game state when a
     * completed round is left behind.
     *
     * @param wallet The player's wallet used for round resolution.
     * @param rng The random number generator used for dealer actions.
     *
     * @return None.
     *
     * @author Alexander
     */
    void onNavigateAway(Wallet& wallet, RNG& rng);

    /**
     * @brief Handles mouse click events on the blackjack screen.
     *
     * This function processes button clicks and performs the appropriate
     * blackjack action depending on the current game state, such as changing
     * the bet, dealing cards, hitting, standing, or starting a new hand.
     *
     * @param p The position of the mouse click.
     * @param wallet The player's wallet used for bets and payouts.
     * @param rng The random number generator used for card draws.
     *
     * @return None.
     *
     * @author Alexander
     */
    void onMouseClick(const sf::Vector2f& p, Wallet& wallet, RNG& rng);

    /**
     * @brief Updates the blackjack screen state.
     *
     * This function performs any ongoing screen updates related to the game,
     * such as ensuring the current bet remains valid with respect to the
     * player's wallet balance.
     *
     * @param wallet The player's wallet used to validate the current bet.
     *
     * @return None.
     *
     * @author Alexander
     */
    void update(const Wallet& wallet);

    /**
     * @brief Updates hover effects for interactive screen elements.
     *
     * This function checks the current mouse position and updates the visual
     * appearance of buttons when the mouse is hovering over them.
     *
     * @param mouse The current mouse position.
     *
     * @return None.
     *
     * @author Alexander
     */
    void updateHover(const sf::Vector2f& mouse);

    /**
     * @brief Draws the blackjack screen.
     *
     * This function renders all visual components of the blackjack interface,
     * including labels, buttons, cards, and status information.
     *
     * @param window The render window used to display the screen.
     *
     * @return None.
     *
     * @author Alexander
     */
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