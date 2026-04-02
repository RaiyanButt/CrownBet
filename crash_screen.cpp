#include "crash_screen.hpp"

#include <iomanip>
#include <sstream>
#include <string>

namespace 
{

/**
 * @brief Formats a multiplier value as a display string.
 *
 * This helper function converts a multiplier into a fixed-point string
 * with two decimal places and appends an 'x' suffix so it can be shown
 * clearly on the crash game screen.
 *
 * @param v The multiplier value to format.
 *
 * @return A formatted multiplier string.
 *
 * @author Daniel
 */
std::string formatMultiplier(double v)
{
    std::ostringstream os;
    os.setf(std::ios::fixed);
    os << std::setprecision(2) << v << 'x';
    return os.str();
}

} // namespace

/**
 * @brief Constructs a CrashScreen object.
 *
 * This constructor initializes the crash screen interface, including
 * buttons, text labels, display panels, and the rocket shape used
 * during gameplay.
 *
 * @param font The font used to render all text on the crash screen.
 *
 * @author Daniel
 */
CrashScreen::CrashScreen(sf::Font& font)
    : rocket_(3),
      betMinus_({75.f, 52.f}, {420.f, 548.f}, sf::Color(220, 220, 220), "-", font, 32),
      betPlus_({75.f, 52.f}, {785.f, 548.f}, sf::Color(220, 220, 220), "+", font, 32),
      mainBtn_({280.f, 54.f}, {500.f, 622.f}, sf::Color(255, 191, 0), "LAUNCH", font, 26)
{
    panel_.setSize(sf::Vector2f(1000.f, 500.f));
    panel_.setPosition(140.f, 128.f);
    panel_.setFillColor(sf::Color(18, 18, 28));
    panel_.setOutlineThickness(4.f);
    panel_.setOutlineColor(sf::Color(255, 191, 0));

    betBox_.setSize(sf::Vector2f(250.f, 52.f));
    betBox_.setPosition(515.f, 548.f);
    betBox_.setFillColor(sf::Color(240, 240, 240));
    betBox_.setOutlineThickness(3.f);
    betBox_.setOutlineColor(sf::Color(255, 191, 0));

    title_.setFont(font);
    title_.setString("Rocket Crash");
    title_.setCharacterSize(44);
    title_.setStyle(sf::Text::Bold);
    title_.setFillColor(sf::Color(255, 191, 0));

    subLine1_.setFont(font);
    subLine1_.setString("Crash point is set when you launch.");
    subLine1_.setCharacterSize(15);
    subLine1_.setFillColor(sf::Color(205, 205, 215));

    subLine2_.setFont(font);
    subLine2_.setString("It stays fixed while the multiplier rises.");
    subLine2_.setCharacterSize(15);
    subLine2_.setFillColor(sf::Color(205, 205, 215));

    subLine3_.setFont(font);
    subLine3_.setString("Cash out before the rocket crashes.");
    subLine3_.setCharacterSize(15);
    subLine3_.setFillColor(sf::Color(205, 205, 215));

    mult_.setFont(font);
    mult_.setCharacterSize(88);
    mult_.setStyle(sf::Text::Bold);
    mult_.setFillColor(sf::Color(255, 235, 100));

    status_.setFont(font);
    status_.setCharacterSize(19);
    status_.setFillColor(sf::Color(238, 238, 245));

    betLabel_.setFont(font);
    betLabel_.setCharacterSize(22);
    betLabel_.setFillColor(sf::Color::Black);
    betLabel_.setStyle(sf::Text::Bold);

    rocket_.setPoint(0, sf::Vector2f(0.f, -42.f));
    rocket_.setPoint(1, sf::Vector2f(20.f, 26.f));
    rocket_.setPoint(2, sf::Vector2f(-20.f, 26.f));
    rocket_.setOrigin(0.f, -42.f);
    rocket_.setFillColor(sf::Color(220, 85, 70));
    rocket_.setOutlineThickness(2.f);
    rocket_.setOutlineColor(sf::Color(140, 40, 35));
}

/**
 * @brief Handles actions required before leaving the crash screen.
 *
 * This function ensures that the current round is properly finalized
 * when the user navigates away from the crash screen. If a round is
 * active, it cashes out automatically; if the round is already over,
 * it resets the game state.
 *
 * @param wallet The player's wallet used for cashing out if necessary.
 *
 * @return None.
 *
 * @author Daniel
 */
void CrashScreen::onNavigateAway(Wallet& wallet)
{
    if (game_.phase() == CrashPhase::Flying) 
    {
        game_.cashOut(wallet);
    } else if (game_.phase() == CrashPhase::RoundOver) 
    {
        game_.acknowledgeRound();
    }
}

/**
 * @brief Handles mouse click input on the crash screen.
 *
 * This function processes clicks on the betting controls and main action
 * button. Depending on the game phase, it may adjust the bet, launch a
 * round, cash out an active round, or advance to the next round.
 *
 * @param p The position of the mouse click.
 * @param wallet The player's wallet used for betting and payouts.
 * @param rng The random number generator used to start a round.
 *
 * @return None.
 *
 * @author Daniel
 */
void CrashScreen::onMouseClick(const sf::Vector2f& p, Wallet& wallet, RNG& rng)
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
    if (!mainBtn_.contains(p)) 
    {
        return;
    }
    if (game_.phase() == CrashPhase::Idle) 
    {
        if (game_.startRound(wallet, rng)) 
        {
            tickClock_.restart();
        }
    } else if (game_.phase() == CrashPhase::Flying) 
    {
        game_.cashOut(wallet);
    } 
    else 
    {
        game_.acknowledgeRound();
    }
}

/**
 * @brief Updates the crash screen state.
 *
 * This function keeps the selected bet valid relative to the wallet,
 * advances the crash game while a round is active, and updates the
 * main button text to match the current phase of gameplay.
 *
 * @param wallet The player's wallet used to validate the current bet.
 *
 * @return None.
 *
 * @author Daniel
 */
void CrashScreen::update(const Wallet& wallet)
{
    game_.clampBetToWallet(wallet);
    if (game_.phase() == CrashPhase::Flying) 
    {
        if (tickClock_.getElapsedTime().asSeconds() >= (5.f / 11.f)) 
        {
            tickClock_.restart();
            game_.tick();
        }
    }
    if (game_.phase() == CrashPhase::Idle) 
    {
        mainBtn_.setText("LAUNCH");
    } 
    else if (game_.phase() == CrashPhase::Flying) 
    {
        mainBtn_.setText("CASH OUT");
    } 
    else 
    {
        mainBtn_.setText("NEXT ROUND");
    }
}

/**
 * @brief Updates button hover effects on the crash screen.
 *
 * This function changes button colors based on the current mouse
 * position to provide visual feedback when the user hovers over
 * interactive controls.
 *
 * @param mouse The current mouse position.
 *
 * @return None.
 *
 * @author Daniel
 */
void CrashScreen::updateHover(const sf::Vector2f& mouse)
{
    betMinus_.setFillColor(betMinus_.contains(mouse) ? sf::Color(255, 255, 255) : sf::Color(220, 220, 220));
    betPlus_.setFillColor(betPlus_.contains(mouse) ? sf::Color(255, 255, 255) : sf::Color(220, 220, 220));
    mainBtn_.setFillColor(mainBtn_.contains(mouse) ? sf::Color(255, 210, 60) : sf::Color(255, 191, 0));
}

/**
 * @brief Draws the crash screen and all interface elements.
 *
 * This function renders the main crash panel, instructional text,
 * current multiplier, game status message, betting controls, rocket
 * graphic, and the primary action button.
 *
 * @param window The render window used to display the crash screen.
 *
 * @return None.
 *
 * @author Daniel
 */
void CrashScreen::draw(sf::RenderWindow& window)
{
    constexpr float cx = 640.f;
    auto centerTextAt = [](sf::Text& t, float x, float y) {
        const sf::FloatRect b = t.getLocalBounds();
        t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
        t.setPosition(x, y);
    };

    centerTextAt(title_, cx, 48.f);

    constexpr float instrLeft = 152.f;
    constexpr float instrTop = 136.f;
    constexpr float instrLineGap = 22.f;
    auto placeLeftInPanel = [](sf::Text& t, float leftX, float topY) {
        const sf::FloatRect b = t.getLocalBounds();
        t.setOrigin(0.f, b.top);
        t.setPosition(leftX, topY);
    };
    placeLeftInPanel(subLine1_, instrLeft, instrTop);
    placeLeftInPanel(subLine2_, instrLeft, instrTop + instrLineGap);
    placeLeftInPanel(subLine3_, instrLeft, instrTop + 2.f * instrLineGap);

    mult_.setString(formatMultiplier(game_.multiplier()));
    centerTextAt(mult_, cx, 298.f);

    std::string msg;
    if (game_.phase() == CrashPhase::Idle) 
    {
        msg = "Use +/- to set bet, then LAUNCH.";
    } 
    else if (game_.phase() == CrashPhase::Flying) 
    {
        const int preview = static_cast<int>(static_cast<double>(game_.roundBet()) * game_.multiplier());
        msg = "CASH OUT now for " + std::to_string(preview) + " coins.";
    } 
    else if (game_.lastOutcome() == CrashOutcome::CashedOut) {
        msg = "Cashed at " + formatMultiplier(game_.multiplier()) + " - +" + std::to_string(game_.lastPayout()) + " coins.";
    } 
    else 
    {
        msg = "Crashed at " + formatMultiplier(game_.crashPoint()) + " - bet lost.";
    }
    status_.setString(msg);
    centerTextAt(status_, cx, 396.f);

    betLabel_.setString("BET: " + std::to_string(game_.currentBet()));
    const sf::FloatRect bb = betBox_.getGlobalBounds();
    const sf::FloatRect tb = betLabel_.getLocalBounds();
    betLabel_.setOrigin(tb.left + tb.width * 0.5f, tb.top + tb.height * 0.5f);
    betLabel_.setPosition(bb.left + bb.width * 0.5f, bb.top + bb.height * 0.5f);

    window.draw(panel_);
    window.draw(title_);
    window.draw(subLine1_);
    window.draw(subLine2_);
    window.draw(subLine3_);
    rocket_.setPosition(cx, 158.f);
    window.draw(rocket_);
    window.draw(mult_);
    window.draw(status_);
    betMinus_.draw(window);
    window.draw(betBox_);
    window.draw(betLabel_);
    betPlus_.draw(window);
    mainBtn_.draw(window);
}