#include "blackjack_screen.hpp"

#include <string>
#include <vector>

namespace 
{

constexpr float kCardW = 58.f;
constexpr float kCardH = 82.f;
constexpr float kCardGap = 10.f;

/**
 * @brief Converts a blackjack card into a display string.
 *
 * This helper function determines what text should appear on a rendered card.
 * If the card is face down, it returns a placeholder symbol. Otherwise, it
 * returns "A" for an ace or the numeric value of the card.
 *
 * @param c The blackjack card being converted.
 * @param faceDown Indicates whether the card should be shown face down.
 *
 * @return A string representing the visible face of the card.
 *
 * @author Alexander
 */
std::string cardFaceString(const BlackjackCard& c, bool faceDown)
{
    if (faceDown)
    {
        return "?";
    }
    if (c.isAce) 
    {
        return "A";
    }
    return std::to_string(c.value);
}

/**
 * @brief Draws a single blackjack card on the screen.
 *
 * This helper function renders one card at the given screen position using
 * the provided font. It adjusts the card appearance depending on whether the
 * card is face down or face up.
 *
 * @param window The render window used for drawing.
 * @param font The font used to render the card text.
 * @param x The x-coordinate where the card should be drawn.
 * @param y The y-coordinate where the card should be drawn.
 * @param c The blackjack card to render.
 * @param faceDown Indicates whether the card should be hidden.
 *
 * @return None.
 *
 * @author Alexander
 */
void drawOneCard(sf::RenderWindow& window,
    sf::Font& font,
    float x,
    float y,
    const BlackjackCard& c,
    bool faceDown)
{
    sf::RectangleShape card(sf::Vector2f(kCardW, kCardH));
    card.setPosition(x, y);

    if (faceDown) 
    {
        card.setFillColor(sf::Color(38, 48, 72));
        card.setOutlineColor(sf::Color(90, 100, 130));
    } \
    else 
    {
        card.setFillColor(sf::Color(252, 252, 255));
        card.setOutlineColor(sf::Color(35, 35, 42));
    }
    card.setOutlineThickness(2.f);
    window.draw(card);

    sf::Text valueText;
    valueText.setFont(font);
    valueText.setString(cardFaceString(c, faceDown));
    const unsigned cs = faceDown ? 38u : (c.isAce ? 36u : 34u);
    valueText.setCharacterSize(cs);
    valueText.setStyle(sf::Text::Bold);
    valueText.setFillColor(faceDown ? sf::Color(230, 235, 245) : sf::Color(28, 28, 36));


    const sf::FloatRect b = valueText.getLocalBounds();
    valueText.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
    valueText.setPosition(x + kCardW * 0.5f, y + kCardH * 0.5f);
    window.draw(valueText);
}

/**
 * @brief Draws a horizontal row of blackjack cards.
 *
 * This helper function renders all cards in a hand centered around the given
 * horizontal position. It can optionally hide the dealer's second card to
 * represent the hole card during gameplay.
 *
 * @param window The render window used for drawing.
 * @param font The font used for rendering card text.
 * @param centerX The horizontal center point of the hand.
 * @param topY The vertical position of the hand.
 * @param hand The vector of blackjack cards to draw.
 * @param hideSecondAsHole Indicates whether the second card should be hidden.
 *
 * @return None.
 *
 * @author Alexander
 */
void drawHandRow(sf::RenderWindow& window,
    sf::Font& font,
    float centerX,
    float topY,
    const std::vector<BlackjackCard>& hand,
    bool hideSecondAsHole)
{
    if (hand.empty()) 
    {
        return;
    }

    const size_t n = hand.size();
    const float rowW = static_cast<float>(n) * kCardW + static_cast<float>(n - 1) * kCardGap;
    float x = centerX - rowW * 0.5f;

    for (size_t i = 0; i < n; ++i) 
    {
        const bool faceDown = hideSecondAsHole && (i == 1);
        drawOneCard(window, font, x, topY, hand[i], faceDown);
        x += kCardW + kCardGap;
    }
}

/**
 * @brief Builds the current status message for the blackjack screen.
 *
 * This helper function returns the text shown to the player based on the
 * current phase and outcome of the blackjack game. It provides either gameplay
 * instructions or a result message after a round ends.
 *
 * @param g The blackjack game whose state is being described.
 *
 * @return A string containing the current status message.
 *
 * @author Alexander
 */
std::string statusLine(const BlackjackGame& g)
{
    switch (g.phase()) 
    {
    case BlackjackPhase::Idle:
        return "Set bet with +/- then press DEAL.";
    case BlackjackPhase::PlayerTurn:
        return "Hit or stand - get closer to 21 than the dealer.";
    case BlackjackPhase::RoundOver:
        break;
    }

    switch (g.outcome()) 
    {
    case BlackjackOutcome::Win:
        if (g.lastWinWasBlackjack()) 
        {
            return "Blackjack pays 2 to 1! Payout " + std::to_string(g.lastPayout()) + " coins.";
        }
        return "You win! Payout " + std::to_string(g.lastPayout()) + " coins (1 to 1).";
    case BlackjackOutcome::Lose:
        return "You lose.";
    case BlackjackOutcome::Push:
        return "Push - bet returned.";
    default:
        return "";
    }
}

/**
 * @brief Builds the caption showing the player's current hand total.
 *
 * This helper function returns a formatted string containing the player's
 * total hand value. If the player has no cards, it returns an empty string
 * so that no caption is shown.
 *
 * @param g The blackjack game whose player total is being displayed.
 *
 * @return A formatted total string for the player's hand.
 *
 * @author Alexander
 */
std::string playerTotalCaption(const BlackjackGame& g)
{
    if (g.playerCards().empty()) 
    {
        return "";
    }
    return "Total " + std::to_string(g.playerTotal());
}

} // namespace

/**
 * @brief Constructs a BlackjackScreen object.
 *
 * This constructor initializes the blackjack screen and all of its visual
 * components, including buttons, labels, panels, and rule text. It also
 * stores the font used for rendering screen elements.
 *
 * @param font The font used to draw all text on the blackjack screen.
 *
 * @author Alexander
 */
BlackjackScreen::BlackjackScreen(sf::Font& font)
    : font_(font),
      betMinus_({75.f, 52.f}, {420.f, 578.f}, sf::Color(220, 220, 220), "-", font, 32),
      betPlus_({75.f, 52.f}, {785.f, 578.f}, sf::Color(220, 220, 220), "+", font, 32),
      dealBtn_({200.f, 52.f}, {540.f, 648.f}, sf::Color(255, 191, 0), "DEAL", font, 26),
      hitBtn_({150.f, 52.f}, {360.f, 648.f}, sf::Color(60, 160, 90), "HIT", font, 26),
      standBtn_({150.f, 52.f}, {770.f, 648.f}, sf::Color(180, 60, 60), "STAND", font, 26),
      newHandBtn_({220.f, 52.f}, {530.f, 648.f}, sf::Color(255, 191, 0), "NEW HAND", font, 26)
{
    panel_.setSize(sf::Vector2f(1000.f, 540.f));
    panel_.setPosition(140.f, 118.f);
    panel_.setFillColor(sf::Color(12, 42, 28));
    panel_.setOutlineThickness(4.f);
    panel_.setOutlineColor(sf::Color(255, 191, 0));

    betBox_.setSize(sf::Vector2f(250.f, 52.f));
    betBox_.setPosition(515.f, 578.f);
    betBox_.setFillColor(sf::Color(240, 240, 240));
    betBox_.setOutlineThickness(3.f);
    betBox_.setOutlineColor(sf::Color(255, 191, 0));

    title_.setFont(font);
    title_.setString("Blackjack");
    title_.setCharacterSize(44);
    title_.setStyle(sf::Text::Bold);
    title_.setFillColor(sf::Color(255, 191, 0));

    payRulesTitle_.setFont(font);
    payRulesTitle_.setString("PAYOUTS");
    payRulesTitle_.setCharacterSize(15);
    payRulesTitle_.setStyle(sf::Text::Bold);
    payRulesTitle_.setFillColor(sf::Color(255, 220, 120));
    payRulesTitle_.setPosition(155.f, 128.f);

    payRulesLine1_.setFont(font);
    payRulesLine1_.setString("Blackjack (2 cards = 21): 2 to 1");
    payRulesLine1_.setCharacterSize(12);
    payRulesLine1_.setFillColor(sf::Color(210, 218, 208));
    payRulesLine1_.setPosition(155.f, 150.f);

    payRulesLine2_.setFont(font);
    payRulesLine2_.setString("Return 3x your bet (2:1 bonus).");
    payRulesLine2_.setCharacterSize(12);
    payRulesLine2_.setFillColor(sf::Color(200, 208, 198));
    payRulesLine2_.setPosition(155.f, 168.f);

    payRulesLine3_.setFont(font);
    payRulesLine3_.setString("Other wins: 1 to 1  (2x bet back).");
    payRulesLine3_.setCharacterSize(12);
    payRulesLine3_.setFillColor(sf::Color(195, 205, 195));
    payRulesLine3_.setPosition(155.f, 186.f);

    cardRulesTitle_.setFont(font);
    cardRulesTitle_.setString("CARDS");
    cardRulesTitle_.setCharacterSize(15);
    cardRulesTitle_.setStyle(sf::Text::Bold);
    cardRulesTitle_.setFillColor(sf::Color(255, 220, 120));

    cardRulesLine1_.setFont(font);
    cardRulesLine1_.setString("Aces count as 1 or 11.");
    cardRulesLine1_.setCharacterSize(12);
    cardRulesLine1_.setFillColor(sf::Color(210, 218, 208));

    cardRulesLine2_.setFont(font);
    cardRulesLine2_.setString("10, J, Q, K count as 10.");
    cardRulesLine2_.setCharacterSize(12);
    cardRulesLine2_.setFillColor(sf::Color(200, 208, 198));

    cardRulesLine3_.setFont(font);
    cardRulesLine3_.setString("Dealer hits to 17.");
    cardRulesLine3_.setCharacterSize(12);
    cardRulesLine3_.setFillColor(sf::Color(195, 205, 195));

    playerLabel_.setFont(font);
    playerLabel_.setString("YOU");
    playerLabel_.setCharacterSize(22);
    playerLabel_.setStyle(sf::Text::Bold);
    playerLabel_.setFillColor(sf::Color(230, 230, 235));

    playerTotal_.setFont(font);
    playerTotal_.setCharacterSize(17);
    playerTotal_.setFillColor(sf::Color(200, 205, 210));

    status_.setFont(font);
    status_.setCharacterSize(19);
    status_.setFillColor(sf::Color(238, 238, 245));

    betLabel_.setFont(font);
    betLabel_.setCharacterSize(22);
    betLabel_.setFillColor(sf::Color::Black);
    betLabel_.setStyle(sf::Text::Bold);
}

/**
 * @brief Updates the blackjack screen state.
 *
 * This function performs per-frame updates related to the blackjack screen.
 * It currently ensures that the game's bet remains valid relative to the
 * player's wallet balance.
 *
 * @param wallet The player's wallet used to validate the current bet.
 *
 * @return None.
 *
 * @author Alexander
 */
void BlackjackScreen::update(const Wallet& wallet)
{
    game_.clampBetToWallet(wallet);
}

/**
 * @brief Handles actions required before leaving the blackjack screen.
 *
 * This function ensures that an active round is resolved before navigating
 * away. If a round is already over, it resets the game so the screen can be
 * cleanly revisited later.
 *
 * @param wallet The player's wallet used for resolving the round if needed.
 * @param rng The random number generator used when resolving dealer actions.
 *
 * @return None.
 *
 * @author Alexander
 */
void BlackjackScreen::onNavigateAway(Wallet& wallet, RNG& rng)
{
    if (game_.phase() == BlackjackPhase::PlayerTurn) 
    {
        game_.stand(wallet, rng);
    }

    if (game_.phase() == BlackjackPhase::RoundOver) 
    {
        game_.newHand();
    }
}

/**
 * @brief Handles mouse click interactions on the blackjack screen.
 *
 * This function checks which control was clicked and performs the appropriate
 * action based on the current phase of the game. It supports changing the bet,
 * dealing a hand, hitting, standing, and starting a new hand.
 *
 * @param p The mouse position where the click occurred.
 * @param wallet The player's wallet used for betting and payouts.
 * @param rng The random number generator used for card draws.
 *
 * @return None.
 *
 * @author Alexander
 */
void BlackjackScreen::onMouseClick(const sf::Vector2f& p, Wallet& wallet, RNG& rng)
{
    if (betMinus_.contains(p)) 
    {
        game_.decreaseBet();
        return;
    }

    if (betPlus_.contains(p)) 
    {
        game_.increaseBet(wallet);
        return;
    }

    if (game_.phase() == BlackjackPhase::Idle) 
    {
        if (dealBtn_.contains(p)) 
        {
            game_.deal(wallet, rng);
        }
        return;
    }

    if (game_.phase() == BlackjackPhase::PlayerTurn) 
    {
        if (hitBtn_.contains(p)) 
        {
            game_.hit(wallet, rng);
        } 
        else if (standBtn_.contains(p)) 
        {
            game_.stand(wallet, rng);
        }
        return;
    }

    if (game_.phase() == BlackjackPhase::RoundOver) 
    {
        if (newHandBtn_.contains(p)) 
        {
            game_.newHand();
        }
    }
}

/**
 * @brief Updates button hover effects based on the mouse position.
 *
 * This function changes the fill colors of interactive buttons to provide
 * visual feedback when the user hovers the mouse over them.
 *
 * @param mouse The current mouse position.
 *
 * @return None.
 *
 * @author Alexander
 */
void BlackjackScreen::updateHover(const sf::Vector2f& mouse)
{
    betMinus_.setFillColor(betMinus_.contains(mouse) ? sf::Color(255, 255, 255) : sf::Color(220, 220, 220));
    betPlus_.setFillColor(betPlus_.contains(mouse) ? sf::Color(255, 255, 255) : sf::Color(220, 220, 220));

    dealBtn_.setFillColor(dealBtn_.contains(mouse) ? sf::Color(255, 210, 60) : sf::Color(255, 191, 0));
    hitBtn_.setFillColor(hitBtn_.contains(mouse) ? sf::Color(90, 200, 120) : sf::Color(60, 160, 90));
    standBtn_.setFillColor(standBtn_.contains(mouse) ? sf::Color(220, 90, 90) : sf::Color(180, 60, 60));
    newHandBtn_.setFillColor(newHandBtn_.contains(mouse) ? sf::Color(255, 210, 60) : sf::Color(255, 191, 0));
}

/**
 * @brief Draws the blackjack screen and all of its UI elements.
 *
 * This function renders the table layout, rule text, player and dealer cards,
 * betting interface, status text, and the appropriate action buttons based on
 * the current game phase.
 *
 * @param window The render window used to display the blackjack screen.
 *
 * @return None.
 *
 * @author Alexander
 */
void BlackjackScreen::draw(sf::RenderWindow& window)
{
    constexpr float cx = 640.f;
    constexpr float dealerCardTop = 208.f;
    constexpr float playerLabelY = dealerCardTop + kCardH + 36.f;
    constexpr float playerCardTop = playerLabelY + 30.f;
    constexpr float playerTotalY = playerCardTop + kCardH + 28.f;
    constexpr float statusY = playerTotalY + 36.f;

    auto centerTextAt = [](sf::Text& t, float x, float y) 
    {
        const sf::FloatRect b = t.getLocalBounds();
        t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
        t.setPosition(x, y);
    };

    centerTextAt(title_, cx, 42.f);

    centerTextAt(playerLabel_, cx, playerLabelY);
    playerTotal_.setString(playerTotalCaption(game_));
    centerTextAt(playerTotal_, cx, playerTotalY);

    status_.setString(statusLine(game_));
    centerTextAt(status_, cx, statusY);

    betLabel_.setString("BET: " + std::to_string(game_.currentBet()));
    const sf::FloatRect bb = betBox_.getGlobalBounds();
    const sf::FloatRect tb = betLabel_.getLocalBounds();
    betLabel_.setOrigin(tb.left + tb.width * 0.5f, tb.top + tb.height * 0.5f);
    betLabel_.setPosition(bb.left + bb.width * 0.5f, bb.top + bb.height * 0.5f);

    window.draw(panel_);
    window.draw(title_);
    window.draw(payRulesTitle_);
    window.draw(payRulesLine1_);
    window.draw(payRulesLine2_);
    window.draw(payRulesLine3_);

    constexpr float feltRightInner = 140.f + 1000.f - 15.f;
    auto placeRightEdge = [](sf::Text& t, float rightX, float topY) 
    {
        const sf::FloatRect b = t.getLocalBounds();
        t.setOrigin(b.left + b.width, 0.f);
        t.setPosition(rightX, topY);
    };
    placeRightEdge(cardRulesTitle_, feltRightInner, 128.f);
    placeRightEdge(cardRulesLine1_, feltRightInner, 150.f);
    placeRightEdge(cardRulesLine2_, feltRightInner, 168.f);
    placeRightEdge(cardRulesLine3_, feltRightInner, 186.f);
    window.draw(cardRulesTitle_);
    window.draw(cardRulesLine1_);
    window.draw(cardRulesLine2_);
    window.draw(cardRulesLine3_);

    drawHandRow(window, font_, cx, dealerCardTop, game_.dealerCards(), game_.dealerHoleHidden());

    window.draw(playerLabel_);

    drawHandRow(window, font_, cx, playerCardTop, game_.playerCards(), false);

    window.draw(playerTotal_);

    window.draw(status_);
    betMinus_.draw(window);
    window.draw(betBox_);
    window.draw(betLabel_);
    betPlus_.draw(window);

    if (game_.phase() == BlackjackPhase::Idle) 
    {
        dealBtn_.draw(window);
    } 
    else if (game_.phase() == BlackjackPhase::PlayerTurn) 
    {
        hitBtn_.draw(window);
        standBtn_.draw(window);
    } 
    else 
    {
        newHandBtn_.draw(window);
    }
}