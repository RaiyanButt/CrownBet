#include "slots_render.hpp"

#include <algorithm>
#include <string>

namespace {

sf::Color slotAccentColor(int symbol)
{
    switch (symbol) {
        case 0: return sf::Color(220, 60, 60);
        case 1: return sf::Color(240, 215, 70);
        case 2: return sf::Color(80, 170, 255);
        case 3: return sf::Color(80, 80, 80);
        case 4: return sf::Color(170, 80, 255);
        default: return sf::Color(200, 200, 200);
    }
}

void drawSlotSymbolAt(sf::RenderWindow& window, int symbol, float cx, float cy, float s, const sf::Font& font)
{
    switch (symbol) {
    case 0: {
        const float r = 24.f * s;
        sf::CircleShape left(r);
        left.setFillColor(sf::Color(195, 30, 45));
        left.setOrigin(r, r);
        left.setPosition(cx - 20.f * s, cy + 6.f * s);
        sf::CircleShape right(r);
        right.setFillColor(sf::Color(215, 45, 60));
        right.setOrigin(r, r);
        right.setPosition(cx + 20.f * s, cy + 6.f * s);
        window.draw(left);
        window.draw(right);

        sf::RectangleShape stem(sf::Vector2f(5.f * s, 42.f * s));
        stem.setFillColor(sf::Color(45, 130, 55));
        stem.setOrigin(2.5f * s, 42.f * s);
        stem.setPosition(cx, cy - 8.f * s);
        stem.setRotation(10.f);
        window.draw(stem);
        break;
    }
    case 1: {
        const float w = 48.f * s;
        const float h = 31.f * s;
        sf::ConvexShape lemon(6);
        lemon.setPoint(0, sf::Vector2f(-w, 0.f));
        lemon.setPoint(1, sf::Vector2f(-w * 0.38f, -h));
        lemon.setPoint(2, sf::Vector2f(w * 0.42f, -h * 0.92f));
        lemon.setPoint(3, sf::Vector2f(w, 0.f));
        lemon.setPoint(4, sf::Vector2f(w * 0.42f, h * 0.92f));
        lemon.setPoint(5, sf::Vector2f(-w * 0.38f, h));
        lemon.setFillColor(sf::Color(252, 234, 95));
        lemon.setOutlineThickness(2.5f * s);
        lemon.setOutlineColor(sf::Color(212, 165, 35));
        lemon.setPosition(cx, cy + 3.f * s);
        window.draw(lemon);

        sf::CircleShape shadow(23.f * s);
        shadow.setFillColor(sf::Color(230, 190, 50));
        shadow.setOrigin(23.f * s, 23.f * s);
        shadow.setScale(1.05f, 0.58f);
        shadow.setPosition(cx + 15.f * s, cy + 12.f * s);
        window.draw(shadow);

        sf::CircleShape gloss(11.f * s);
        gloss.setFillColor(sf::Color(255, 252, 220));
        gloss.setOrigin(11.f * s, 11.f * s);
        gloss.setScale(1.35f, 0.55f);
        gloss.setPosition(cx - 16.f * s, cy - 14.f * s);
        window.draw(gloss);
        break;
    }
    case 2: {
        sf::ConvexShape skirt(4);
        skirt.setPoint(0, sf::Vector2f(-24.f * s, 0.f));
        skirt.setPoint(1, sf::Vector2f(24.f * s, 0.f));
        skirt.setPoint(2, sf::Vector2f(44.f * s, 40.f * s));
        skirt.setPoint(3, sf::Vector2f(-44.f * s, 40.f * s));
        skirt.setFillColor(sf::Color(235, 175, 45));
        skirt.setOutlineThickness(2.5f * s);
        skirt.setOutlineColor(sf::Color(145, 95, 20));
        skirt.setPosition(cx, cy + 2.f * s);
        window.draw(skirt);

        const float domeR = 34.f * s;
        sf::CircleShape dome(domeR);
        dome.setFillColor(sf::Color(255, 224, 110));
        dome.setOutlineThickness(2.5f * s);
        dome.setOutlineColor(sf::Color(175, 115, 25));
        dome.setOrigin(domeR, domeR * 0.72f);
        dome.setScale(1.f, 0.9f);
        dome.setPosition(cx, cy - 4.f * s);
        window.draw(dome);

        sf::CircleShape knob(7.f * s);
        knob.setFillColor(sf::Color(220, 165, 40));
        knob.setOutlineThickness(1.5f * s);
        knob.setOutlineColor(sf::Color(120, 75, 15));
        knob.setOrigin(7.f * s, 7.f * s);
        knob.setPosition(cx, cy - 38.f * s);
        window.draw(knob);

        sf::RectangleShape loop(sf::Vector2f(5.f * s, 12.f * s));
        loop.setFillColor(sf::Color(200, 145, 35));
        loop.setOutlineThickness(1.f * s);
        loop.setOutlineColor(sf::Color(110, 70, 12));
        loop.setOrigin(2.5f * s, 12.f * s);
        loop.setPosition(cx, cy - 44.f * s);
        window.draw(loop);

        sf::CircleShape clapper(6.f * s);
        clapper.setFillColor(sf::Color(210, 155, 45));
        clapper.setOutlineThickness(1.5f * s);
        clapper.setOutlineColor(sf::Color(120, 78, 18));
        clapper.setOrigin(6.f * s, 6.f * s);
        clapper.setPosition(cx, cy + 46.f * s);
        window.draw(clapper);
        break;
    }
    case 3: {
        const float w = 108.f * s;
        const float h = 13.f * s;
        const float gap = 9.f * s;
        const float totalH = 3.f * h + 2.f * gap;
        const float y0 = cy - totalH * 0.5f;
        for (int i = 0; i < 3; ++i) {
            sf::RectangleShape bar(sf::Vector2f(w, h));
            bar.setFillColor(sf::Color(52, 52, 52));
            bar.setOutlineThickness(2.f * s);
            bar.setOutlineColor(sf::Color(28, 28, 28));
            bar.setPosition(cx - w * 0.5f, y0 + static_cast<float>(i) * (h + gap));
            window.draw(bar);
        }
        break;
    }
    case 4: {
        sf::Text seven;
        seven.setFont(font);
        seven.setString("7");
        const unsigned int cs = static_cast<unsigned int>(86.f * s);
        seven.setCharacterSize(cs);
        seven.setStyle(sf::Text::Bold);
        sf::FloatRect tb = seven.getLocalBounds();
        seven.setOrigin(tb.left + tb.width * 0.5f, tb.top + tb.height * 0.5f);

        const float outline = std::max(2.f, s * 3.8f);
        const sf::Color outlineCol(45, 12, 18);
        const sf::Color faceCol(255, 55, 75);
        const sf::Color sheenCol(255, 235, 200);

        const sf::Vector2f offsets[] = {
            {-outline, 0.f},
            {outline, 0.f},
            {0.f, -outline},
            {0.f, outline},
            {-outline * 0.72f, -outline * 0.72f},
            {outline * 0.72f, -outline * 0.72f},
            {-outline * 0.72f, outline * 0.72f},
            {outline * 0.72f, outline * 0.72f},
        };

        for (const sf::Vector2f& off : offsets) {
            seven.setFillColor(outlineCol);
            seven.setPosition(cx + off.x, cy + off.y);
            window.draw(seven);
        }

        seven.setFillColor(faceCol);
        seven.setPosition(cx - outline * 0.28f, cy - outline * 0.35f);
        window.draw(seven);

        const unsigned int sheenCs = static_cast<unsigned int>(std::max(22.f, cs * 0.42f));
        seven.setCharacterSize(sheenCs);
        tb = seven.getLocalBounds();
        seven.setOrigin(tb.left + tb.width * 0.5f, tb.top + tb.height * 0.5f);
        seven.setFillColor(sheenCol);
        seven.setPosition(cx - cs * 0.11f, cy - cs * 0.2f);
        window.draw(seven);
        break;
    }
    default: {
        sf::Text unk;
        unk.setFont(font);
        unk.setString("?");
        unk.setCharacterSize(static_cast<unsigned int>(48.f * s));
        unk.setFillColor(sf::Color(120, 120, 120));
        const sf::FloatRect ub = unk.getLocalBounds();
        unk.setOrigin(ub.left + ub.width * 0.5f, ub.top + ub.height * 0.5f);
        unk.setPosition(cx, cy);
        window.draw(unk);
        break;
    }
    }
}

void drawPayoutEllipsis(sf::RenderWindow& window, float cx, float cy, float s)
{
    const float r = std::max(2.f, 3.2f * s);
    for (int i = -1; i <= 1; ++i) {
        sf::CircleShape dot(r);
        dot.setFillColor(sf::Color(255, 205, 90));
        dot.setOrigin(r, r);
        dot.setPosition(cx + static_cast<float>(i) * 8.f * s, cy);
        window.draw(dot);
    }
}

void drawPayoutPrize(sf::RenderWindow& window,
    const sf::Font& font,
    float rightX,
    float cy,
    int mult,
    int bet,
    float rowH)
{
    const int coins = bet * mult;
    const unsigned int csTop = static_cast<unsigned int>(std::max(12.f, std::min(16.f, rowH * 0.30f)));
    const unsigned int csBot = static_cast<unsigned int>(std::max(10.f, std::min(14.f, rowH * 0.26f)));

    sf::Text multLine;
    multLine.setFont(font);
    multLine.setCharacterSize(csTop);
    multLine.setStyle(sf::Text::Bold);
    multLine.setFillColor(sf::Color(255, 228, 130));
    multLine.setString(std::to_string(mult) + "x bet");
    const sf::FloatRect mb = multLine.getLocalBounds();
    multLine.setOrigin(mb.left + mb.width, mb.top + mb.height * 0.5f);
    multLine.setPosition(rightX, cy - rowH * 0.15f);
    window.draw(multLine);

    sf::Text coinLine;
    coinLine.setFont(font);
    coinLine.setCharacterSize(csBot);
    coinLine.setFillColor(sf::Color::White);
    coinLine.setString(std::to_string(coins) + " coins");
    const sf::FloatRect cb = coinLine.getLocalBounds();
    coinLine.setOrigin(cb.left + cb.width, cb.top + cb.height * 0.5f);
    coinLine.setPosition(rightX, cy + rowH * 0.16f);
    window.draw(coinLine);
}

} // namespace

void centerTextInBox(sf::Text& text, const sf::RectangleShape& box)
{
    const sf::FloatRect textBounds = text.getLocalBounds();
    const sf::FloatRect boxBounds = box.getGlobalBounds();

    text.setOrigin(
        textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height / 2.f);

    text.setPosition(
        boxBounds.left + boxBounds.width / 2.f,
        boxBounds.top + boxBounds.height / 2.f);
}

void styleReel(sf::RectangleShape& reel, int symbol)
{
    reel.setFillColor(sf::Color(245, 245, 245));
    reel.setOutlineThickness(6.f);
    reel.setOutlineColor(slotAccentColor(symbol));
}

void drawSlotSymbol(sf::RenderWindow& window, int symbol, const sf::RectangleShape& inner, const sf::Font& font)
{
    const sf::FloatRect b = inner.getGlobalBounds();
    const float cx = b.left + b.width * 0.5f;
    const float cy = b.top + b.height * 0.5f;
    const float s = std::min(b.width, b.height) / 150.f;
    drawSlotSymbolAt(window, symbol, cx, cy, s, font);
}

void drawPayoutTable(sf::RenderWindow& window, const sf::Font& font, const sf::RectangleShape& panel, int currentBet)
{
    const sf::FloatRect pb = panel.getGlobalBounds();
    const float padX = 12.f;
    const float padYBottom = 12.f;
    const float headerBand = 52.f;
    const float contentTop = pb.top + headerBand;
    const float contentBottom = pb.top + pb.height - padYBottom;
    const float contentH = std::max(80.f, contentBottom - contentTop);
    constexpr int rowCount = 6;
    const float rowH = contentH / static_cast<float>(rowCount);

    const float symS = std::min(0.42f, std::max(0.26f, rowH / 100.f));

    const float symbolsLeft = pb.left + padX;
    const float symbolsRight = pb.left + pb.width * 0.54f;
    const float symBandW = std::max(40.f, symbolsRight - symbolsLeft);
    const float x1 = symbolsLeft + symBandW * 0.17f;
    const float x2 = symbolsLeft + symBandW * 0.5f;
    const float x3 = symbolsLeft + symBandW * 0.83f;
    const float rightX = pb.left + pb.width - padX;

    const auto rowY = [&](int i) {
        return contentTop + (static_cast<float>(i) + 0.5f) * rowH;
    };

    drawPayoutPrize(window, font, rightX, rowY(0), 10, currentBet, rowH);
    drawSlotSymbolAt(window, 4, x1, rowY(0), symS, font);
    drawSlotSymbolAt(window, 4, x2, rowY(0), symS, font);
    drawSlotSymbolAt(window, 4, x3, rowY(0), symS, font);

    drawPayoutPrize(window, font, rightX, rowY(1), 6, currentBet, rowH);
    drawSlotSymbolAt(window, 3, x1, rowY(1), symS, font);
    drawSlotSymbolAt(window, 3, x2, rowY(1), symS, font);
    drawSlotSymbolAt(window, 3, x3, rowY(1), symS, font);

    drawPayoutPrize(window, font, rightX, rowY(2), 4, currentBet, rowH);
    drawSlotSymbolAt(window, 2, x1, rowY(2), symS, font);
    drawSlotSymbolAt(window, 2, x2, rowY(2), symS, font);
    drawSlotSymbolAt(window, 2, x3, rowY(2), symS, font);

    drawPayoutPrize(window, font, rightX, rowY(3), 3, currentBet, rowH);
    drawSlotSymbolAt(window, 1, x1, rowY(3), symS, font);
    drawSlotSymbolAt(window, 1, x2, rowY(3), symS, font);
    drawSlotSymbolAt(window, 1, x3, rowY(3), symS, font);

    drawPayoutPrize(window, font, rightX, rowY(4), 3, currentBet, rowH);
    drawSlotSymbolAt(window, 0, x1, rowY(4), symS, font);
    drawSlotSymbolAt(window, 0, x2, rowY(4), symS, font);
    drawSlotSymbolAt(window, 0, x3, rowY(4), symS, font);

    drawPayoutPrize(window, font, rightX, rowY(5), 2, currentBet, rowH);
    const float duoSpread = symBandW * 0.2f;
    drawSlotSymbolAt(window, 0, x2 - duoSpread, rowY(5), symS, font);
    drawSlotSymbolAt(window, 0, x2 + duoSpread, rowY(5), symS, font);
    const float ellipX = std::min(pb.left + pb.width * 0.62f, rightX - rowH * 0.85f);
    drawPayoutEllipsis(window, ellipX, rowY(5), symS);
}

std::string resultMessageFromSpin(const SlotsResult& result, int currentBet, bool validBet)
{
    if (!validBet) {
        return "Bet exceeds balance.";
    }

    if (result.won) {
        return "WIN! +" + std::to_string(result.payout) + " coins (" + std::to_string(result.multiplier) + "x)";
    }

    return "No win. -" + std::to_string(currentBet) + " coins";
}
