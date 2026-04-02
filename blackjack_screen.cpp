#include "blackjack_screen.hpp"

#include <string>
#include <vector>

namespace {

constexpr float kCardW = 58.f;
constexpr float kCardH = 82.f;
constexpr float kCardGap = 10.f;

std::string cardFaceString(const BlackjackCard& c, bool faceDown)
{
    if (faceDown) {
        return "?";
    }
    if (c.isAce) {
        return "A";
    }
    return std::to_string(c.value);
}

void drawOneCard(sf::RenderWindow& window,
    sf::Font& font,
    float x,
    float y,
    const BlackjackCard& c,
    bool faceDown)
{
    sf::RectangleShape card(sf::Vector2f(kCardW, kCardH));
    card.setPosition(x, y);
    if (faceDown) {
        card.setFillColor(sf::Color(38, 48, 72));
        card.setOutlineColor(sf::Color(90, 100, 130));
    } else {
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

void drawHandRow(sf::RenderWindow& window,
    sf::Font& font,
    float centerX,
    float topY,
    const std::vector<BlackjackCard>& hand,
    bool hideSecondAsHole)
{
    if (hand.empty()) {
        return;
    }

    const size_t n = hand.size();
    const float rowW = static_cast<float>(n) * kCardW + static_cast<float>(n - 1) * kCardGap;
    float x = centerX - rowW * 0.5f;

    for (size_t i = 0; i < n; ++i) {
        const bool faceDown = hideSecondAsHole && (i == 1);
        drawOneCard(window, font, x, topY, hand[i], faceDown);
        x += kCardW + kCardGap;
    }
}

std::string statusLine(const BlackjackGame& g)
{
    switch (g.phase()) {
    case BlackjackPhase::Idle:
        return "Set bet with +/- then press DEAL.";
    case BlackjackPhase::PlayerTurn:
        return "Hit or stand - get closer to 21 than the dealer.";
    case BlackjackPhase::RoundOver:
        break;
    }

    switch (g.outcome()) {
    case BlackjackOutcome::Win:
        if (g.lastWinWasBlackjack()) {
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

std::string playerTotalCaption(const BlackjackGame& g)
{
    if (g.playerCards().empty()) {
        return "";
    }
    return "Total " + std::to_string(g.playerTotal());
}

} // namespace

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

void BlackjackScreen::update(const Wallet& wallet)
{
    game_.clampBetToWallet(wallet);
}

void BlackjackScreen::onNavigateAway(Wallet& wallet, RNG& rng)
{
    if (game_.phase() == BlackjackPhase::PlayerTurn) {
        game_.stand(wallet, rng);
    }
    if (game_.phase() == BlackjackPhase::RoundOver) {
        game_.newHand();
    }
}

void BlackjackScreen::onMouseClick(const sf::Vector2f& p, Wallet& wallet, RNG& rng)
{
    if (betMinus_.contains(p)) {
        game_.decreaseBet();
        return;
    }
    if (betPlus_.contains(p)) {
        game_.increaseBet(wallet);
        return;
    }

    if (game_.phase() == BlackjackPhase::Idle) {
        if (dealBtn_.contains(p)) {
            game_.deal(wallet, rng);
        }
        return;
    }

    if (game_.phase() == BlackjackPhase::PlayerTurn) {
        if (hitBtn_.contains(p)) {
            game_.hit(wallet, rng);
        } else if (standBtn_.contains(p)) {
            game_.stand(wallet, rng);
        }
        return;
    }

    if (game_.phase() == BlackjackPhase::RoundOver) {
        if (newHandBtn_.contains(p)) {
            game_.newHand();
        }
    }
}

void BlackjackScreen::updateHover(const sf::Vector2f& mouse)
{
    betMinus_.setFillColor(betMinus_.contains(mouse) ? sf::Color(255, 255, 255) : sf::Color(220, 220, 220));
    betPlus_.setFillColor(betPlus_.contains(mouse) ? sf::Color(255, 255, 255) : sf::Color(220, 220, 220));

    dealBtn_.setFillColor(dealBtn_.contains(mouse) ? sf::Color(255, 210, 60) : sf::Color(255, 191, 0));
    hitBtn_.setFillColor(hitBtn_.contains(mouse) ? sf::Color(90, 200, 120) : sf::Color(60, 160, 90));
    standBtn_.setFillColor(standBtn_.contains(mouse) ? sf::Color(220, 90, 90) : sf::Color(180, 60, 60));
    newHandBtn_.setFillColor(newHandBtn_.contains(mouse) ? sf::Color(255, 210, 60) : sf::Color(255, 191, 0));
}

void BlackjackScreen::draw(sf::RenderWindow& window)
{
    constexpr float cx = 640.f;
    constexpr float dealerCardTop = 208.f;
    constexpr float playerLabelY = dealerCardTop + kCardH + 36.f;
    constexpr float playerCardTop = playerLabelY + 30.f;
    constexpr float playerTotalY = playerCardTop + kCardH + 28.f;
    constexpr float statusY = playerTotalY + 36.f;

    auto centerTextAt = [](sf::Text& t, float x, float y) {
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
    auto placeRightEdge = [](sf::Text& t, float rightX, float topY) {
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

    if (game_.phase() == BlackjackPhase::Idle) {
        dealBtn_.draw(window);
    } else if (game_.phase() == BlackjackPhase::PlayerTurn) {
        hitBtn_.draw(window);
        standBtn_.draw(window);
    } else {
        newHandBtn_.draw(window);
    }
}
