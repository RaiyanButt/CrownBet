#include "crash_screen.hpp"

#include <iomanip>
#include <sstream>
#include <string>

namespace {

std::string formatMultiplier(double v)
{
    std::ostringstream os;
    os.setf(std::ios::fixed);
    os << std::setprecision(2) << v << 'x';
    return os.str();
}

} // namespace

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
    subLine1_.setString("Crash point is set when the round starts (like FlyX). Cash out in time.");
    subLine1_.setCharacterSize(17);
    subLine1_.setFillColor(sf::Color(205, 205, 215));

    subLine2_.setFont(font);
    subLine2_.setString("Cash out before the rocket crashes.");
    subLine2_.setCharacterSize(17);
    subLine2_.setFillColor(sf::Color(205, 205, 215));

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
    rocket_.setFillColor(sf::Color(220, 85, 70));
    rocket_.setOutlineThickness(2.f);
    rocket_.setOutlineColor(sf::Color(140, 40, 35));
}

void CrashScreen::onNavigateAway(Wallet& wallet)
{
    if (game_.phase() == CrashPhase::Flying) {
        game_.cashOut(wallet);
    } else if (game_.phase() == CrashPhase::RoundOver) {
        game_.acknowledgeRound();
    }
}

void CrashScreen::onMouseClick(const sf::Vector2f& p, Wallet& wallet, RNG& rng)
{
    if (betMinus_.contains(p)) {
        game_.decreaseBet();
        return;
    }
    if (betPlus_.contains(p)) {
        game_.increaseBet(wallet);
        return;
    }
    if (!mainBtn_.contains(p)) {
        return;
    }
    if (game_.phase() == CrashPhase::Idle) {
        if (game_.startRound(wallet, rng)) {
            tickClock_.restart();
        }
    } else if (game_.phase() == CrashPhase::Flying) {
        game_.cashOut(wallet);
    } else {
        game_.acknowledgeRound();
    }
}

void CrashScreen::update(const Wallet& wallet)
{
    game_.clampBetToWallet(wallet);
    if (game_.phase() == CrashPhase::Flying) {
        if (tickClock_.getElapsedTime().asSeconds() >= (5.f / 11.f)) {
            tickClock_.restart();
            game_.tick();
        }
    }
    if (game_.phase() == CrashPhase::Idle) {
        mainBtn_.setText("LAUNCH");
    } else if (game_.phase() == CrashPhase::Flying) {
        mainBtn_.setText("CASH OUT");
    } else {
        mainBtn_.setText("NEXT ROUND");
    }
}

void CrashScreen::updateHover(const sf::Vector2f& mouse)
{
    betMinus_.setFillColor(betMinus_.contains(mouse) ? sf::Color(255, 255, 255) : sf::Color(220, 220, 220));
    betPlus_.setFillColor(betPlus_.contains(mouse) ? sf::Color(255, 255, 255) : sf::Color(220, 220, 220));
    mainBtn_.setFillColor(mainBtn_.contains(mouse) ? sf::Color(255, 210, 60) : sf::Color(255, 191, 0));
}

void CrashScreen::draw(sf::RenderWindow& window)
{
    constexpr float cx = 640.f;
    auto centerTextAt = [](sf::Text& t, float x, float y) {
        const sf::FloatRect b = t.getLocalBounds();
        t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
        t.setPosition(x, y);
    };

    centerTextAt(title_, cx, 48.f);
    centerTextAt(subLine1_, cx, 84.f);
    centerTextAt(subLine2_, cx, 104.f);

    mult_.setString(formatMultiplier(game_.multiplier()));
    centerTextAt(mult_, cx, 298.f);

    std::string msg;
    if (game_.phase() == CrashPhase::Idle) {
        msg = "Use +/- to set bet, then LAUNCH.";
    } else if (game_.phase() == CrashPhase::Flying) {
        const int preview = static_cast<int>(static_cast<double>(game_.roundBet()) * game_.multiplier());
        msg = "CASH OUT now for " + std::to_string(preview) + " coins.";
    } else if (game_.lastOutcome() == CrashOutcome::CashedOut) {
        msg = "Cashed at " + formatMultiplier(game_.multiplier()) + " — +" + std::to_string(game_.lastPayout()) + " coins.";
    } else {
        msg = "Crashed at " + formatMultiplier(game_.crashPoint()) + " — bet lost.";
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
    rocket_.setPosition(640.f, 176.f);
    window.draw(rocket_);
    window.draw(mult_);
    window.draw(status_);
    betMinus_.draw(window);
    window.draw(betBox_);
    window.draw(betLabel_);
    betPlus_.draw(window);
    mainBtn_.draw(window);
}
