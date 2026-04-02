#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "app_types.hpp"
#include "button.hpp"
#include "blackjack_screen.hpp"
#include "crash_screen.hpp"
#include "rng.hpp"
#include "slots_game.hpp"
#include "slots_render.hpp"
#include "wallet.hpp"

/**
 * @brief Entry point of the CrownBet casino application.
 *
 * This function initializes the main SFML window, loads required assets,
 * creates all game screens and shared game objects, and runs the main
 * application loop. It handles user input, updates the active screen,
 * manages screen navigation, and renders the appropriate interface for
 * the main menu, slots, blackjack, and crash games.
 *
 * @return Exit status code indicating whether the program ended successfully.
 *
 * @author Raiyan
 */
int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "CrownBet");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("assets/Geneva.ttf")) 
    {
        std::cerr << "Could not load font: assets/Geneva.ttf\n";
        return 1;
    }

    RNG rng;
    Wallet wallet(1000);
    SlotsGame slotsGame(50);
    BlackjackScreen blackjackScreen(font);
    CrashScreen crashScreen(font);
    Screen currentScreen = Screen::MainMenu;

    int reelA = 0;
    int reelB = 1;
    int reelC = 2;
    std::string slotsMessage = "Press SPIN to play.";

    sf::Text title;
    title.setFont(font);
    title.setString("CrownBet");
    title.setCharacterSize(64);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color(255, 191, 0));
    title.setPosition(500.f, 35.f);

    sf::Text subtitle;
    subtitle.setFont(font);
    subtitle.setString("Casino Lobby");
    subtitle.setCharacterSize(24);
    subtitle.setFillColor(sf::Color::White);
    subtitle.setPosition(560.f, 110.f);

    sf::RectangleShape balanceBox(sf::Vector2f(330.f, 58.f));
    balanceBox.setPosition(885.f, 28.f);
    balanceBox.setFillColor(sf::Color(255, 191, 0));
    balanceBox.setOutlineThickness(3.f);
    balanceBox.setOutlineColor(sf::Color(220, 220, 220));

    sf::Text balanceText;
    balanceText.setFont(font);
    balanceText.setCharacterSize(22);
    balanceText.setFillColor(sf::Color::Black);
    balanceText.setStyle(sf::Text::Bold);

    Button blackjackCard({220.f, 290.f}, {120.f, 210.f}, sf::Color(220, 220, 220), "BLACKJACK", font, 28);
    Button slotsCard({220.f, 290.f}, {530.f, 210.f}, sf::Color(245, 245, 245), "SLOTS", font, 28);
    Button crashCard({220.f, 290.f}, {940.f, 210.f}, sf::Color(220, 220, 220), "CRASH", font, 28);

    Button blackjackPlay({140.f, 50.f}, {160.f, 420.f}, sf::Color(255, 191, 0), "PLAY", font, 28);
    Button slotsPlay({140.f, 50.f}, {570.f, 420.f}, sf::Color(255, 191, 0), "PLAY", font, 28);
    Button crashPlay({140.f, 50.f}, {980.f, 420.f}, sf::Color(255, 191, 0), "PLAY", font, 28);
    Button exitButton({320.f, 70.f}, {480.f, 580.f}, sf::Color::Red, "EXIT", font, 30);

    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(26);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(40.f, 665.f);
    statusText.setString("");

    sf::Text slotsTitle;
    slotsTitle.setFont(font);
    slotsTitle.setString("Slot Machine");
    slotsTitle.setCharacterSize(52);
    slotsTitle.setStyle(sf::Text::Bold);
    slotsTitle.setFillColor(sf::Color(255, 191, 0));

    sf::Text slotsSubheading;
    slotsSubheading.setFont(font);
    slotsSubheading.setString("Match symbols to win multipliers");
    slotsSubheading.setCharacterSize(22);
    slotsSubheading.setFillColor(sf::Color(220, 220, 220));

    Button backButton({180.f, 55.f}, {46.f, 30.f}, sf::Color(175, 175, 182), "BACK", font, 24);
    Button betMinus({75.f, 58.f}, {420.f, 575.f}, sf::Color(220, 220, 220), "-", font, 34);
    Button betPlus({75.f, 58.f}, {785.f, 575.f}, sf::Color(220, 220, 220), "+", font, 34);
    Button spinButton({220.f, 76.f}, {530.f, 632.f}, sf::Color(255, 191, 0), "SPIN", font, 30);

    sf::RectangleShape slotsPanel(sf::Vector2f(1110.f, 540.f));
    slotsPanel.setPosition(85.f, 135.f);
    slotsPanel.setFillColor(sf::Color(20, 20, 20));
    slotsPanel.setOutlineThickness(4.f);
    slotsPanel.setOutlineColor(sf::Color(255, 191, 0));

    sf::RectangleShape messagePanel(sf::Vector2f(440.f, 56.f));
    messagePanel.setPosition(420.f, 152.f);
    messagePanel.setFillColor(sf::Color(35, 35, 35));
    messagePanel.setOutlineThickness(2.f);
    messagePanel.setOutlineColor(sf::Color(255, 191, 0));

    sf::Text slotsMessageText;
    slotsMessageText.setFont(font);
    slotsMessageText.setCharacterSize(21);
    slotsMessageText.setFillColor(sf::Color::White);

    sf::RectangleShape payoutPanel(sf::Vector2f(316.f, 444.f));
    payoutPanel.setPosition(868.f, 226.f);
    payoutPanel.setFillColor(sf::Color(35, 35, 35));
    payoutPanel.setOutlineThickness(3.f);
    payoutPanel.setOutlineColor(sf::Color(255, 191, 0));

    sf::Text payoutTitle;
    payoutTitle.setFont(font);
    payoutTitle.setString("PAYOUTS");
    payoutTitle.setCharacterSize(28);
    payoutTitle.setStyle(sf::Text::Bold);
    payoutTitle.setFillColor(sf::Color(255, 191, 0));

    sf::RectangleShape reel1(sf::Vector2f(200.f, 220.f));
    reel1.setPosition(145.f, 275.f);

    sf::RectangleShape reel2(sf::Vector2f(200.f, 220.f));
    reel2.setPosition(390.f, 275.f);

    sf::RectangleShape reel3(sf::Vector2f(200.f, 220.f));
    reel3.setPosition(635.f, 275.f);

    sf::RectangleShape reelInner1(sf::Vector2f(170.f, 150.f));
    reelInner1.setPosition(160.f, 325.f);
    reelInner1.setFillColor(sf::Color::White);

    sf::RectangleShape reelInner2(sf::Vector2f(170.f, 150.f));
    reelInner2.setPosition(405.f, 325.f);
    reelInner2.setFillColor(sf::Color::White);

    sf::RectangleShape reelInner3(sf::Vector2f(170.f, 150.f));
    reelInner3.setPosition(650.f, 325.f);
    reelInner3.setFillColor(sf::Color::White);

    sf::Text reelLabel1;
    reelLabel1.setFont(font);
    reelLabel1.setString("REEL 1");
    reelLabel1.setCharacterSize(18);
    reelLabel1.setStyle(sf::Text::Bold);
    reelLabel1.setFillColor(sf::Color::Black);
    reelLabel1.setPosition(210.f, 285.f);

    sf::Text reelLabel2;
    reelLabel2.setFont(font);
    reelLabel2.setString("REEL 2");
    reelLabel2.setCharacterSize(18);
    reelLabel2.setStyle(sf::Text::Bold);
    reelLabel2.setFillColor(sf::Color::Black);
    reelLabel2.setPosition(455.f, 285.f);

    sf::Text reelLabel3;
    reelLabel3.setFont(font);
    reelLabel3.setString("REEL 3");
    reelLabel3.setCharacterSize(18);
    reelLabel3.setStyle(sf::Text::Bold);
    reelLabel3.setFillColor(sf::Color::Black);
    reelLabel3.setPosition(700.f, 285.f);

    sf::RectangleShape betBox(sf::Vector2f(250.f, 58.f));
    betBox.setPosition(515.f, 575.f);
    betBox.setFillColor(sf::Color(240, 240, 240));
    betBox.setOutlineThickness(3.f);
    betBox.setOutlineColor(sf::Color(255, 191, 0));

    sf::Text betText;
    betText.setFont(font);
    betText.setCharacterSize(24);
    betText.setFillColor(sf::Color::Black);
    betText.setStyle(sf::Text::Bold);

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) 
                {
                const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (currentScreen == Screen::MainMenu) 
                {
                    if (slotsPlay.contains(mousePos)) 
                    {
                        currentScreen = Screen::Slots;
                    } 
                    else if (blackjackPlay.contains(mousePos)) 
                    {
                        currentScreen = Screen::Blackjack;
                    } 
                    else if (crashPlay.contains(mousePos)) 
                    {
                        currentScreen = Screen::Crash;
                    } 
                    else if (exitButton.contains(mousePos)) 
                    {
                        window.close();
                    }
                } 
                else if (currentScreen == Screen::Slots) 
                {
                    if (backButton.contains(mousePos)) 
                    {
                        currentScreen = Screen::MainMenu;
                    } 
                    else if (betMinus.contains(mousePos)) 
                    {
                        slotsGame.decreaseBet();
                    } 
                    else if (betPlus.contains(mousePos)) 
                    {
                        slotsGame.increaseBet(wallet);
                    } 
                    else if (spinButton.contains(mousePos)) 
                    {
                        const bool validBet = wallet.canBet(slotsGame.currentBet());
                        const int betBeforeSpin = slotsGame.currentBet();
                        SlotsResult result = slotsGame.spin(wallet, rng);

                        if (validBet) 
                        {
                            reelA = result.reelA;
                            reelB = result.reelB;
                            reelC = result.reelC;
                        }

                        slotsMessage = resultMessageFromSpin(result, betBeforeSpin, validBet);
                    }
                } 
                else if (currentScreen == Screen::Blackjack) 
                {
                    if (backButton.contains(mousePos)) 
                    {
                        blackjackScreen.onNavigateAway(wallet, rng);
                        currentScreen = Screen::MainMenu;
                    }
                    else 
                    {
                        blackjackScreen.onMouseClick(mousePos, wallet, rng);
                    }
                } 
                else if (currentScreen == Screen::Crash) 
                {
                    if (backButton.contains(mousePos)) 
                    {
                        crashScreen.onNavigateAway(wallet);
                        currentScreen = Screen::MainMenu;
                    } 
                    else 
                    {
                        crashScreen.onMouseClick(mousePos, wallet, rng);
                    }
                }
            }
        }

        if (currentScreen == Screen::Blackjack) 
        {
            blackjackScreen.update(wallet);
        }
        if (currentScreen == Screen::Crash) 
        {
            crashScreen.update(wallet);
        }

        balanceText.setString("BALANCE: " + std::to_string(wallet.balance()) + " COINS");
        centerTextInBox(balanceText, balanceBox);

        if (currentScreen == Screen::Slots) 
        {
            betText.setString("BET: " + std::to_string(slotsGame.currentBet()));
            centerTextInBox(betText, betBox);
        } 
        else if (currentScreen != Screen::Crash) 
        {
            betText.setString("BET: " + std::to_string(slotsGame.currentBet()));
            centerTextInBox(betText, betBox);
        }

        slotsMessageText.setString(slotsMessage);
        centerTextInBox(slotsMessageText, messagePanel);

        styleReel(reel1, reelA);
        styleReel(reel2, reelB);
        styleReel(reel3, reelC);

        const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        slotsPlay.setFillColor(slotsPlay.contains(mousePos) ? sf::Color(255, 210, 60) : sf::Color(255, 191, 0));
        blackjackPlay.setFillColor(blackjackPlay.contains(mousePos) ? sf::Color(255, 210, 60) : sf::Color(255, 191, 0));
        crashPlay.setFillColor(crashPlay.contains(mousePos) ? sf::Color(255, 210, 60) : sf::Color(255, 191, 0));
        exitButton.setFillColor(exitButton.contains(mousePos) ? sf::Color(255, 80, 80) : sf::Color::Red);

        backButton.setFillColor(backButton.contains(mousePos) ? sf::Color(210, 210, 218) : sf::Color(175, 175, 182));
        betMinus.setFillColor(betMinus.contains(mousePos) ? sf::Color(255, 255, 255) : sf::Color(220, 220, 220));
        betPlus.setFillColor(betPlus.contains(mousePos) ? sf::Color(255, 255, 255) : sf::Color(220, 220, 220));
        spinButton.setFillColor(spinButton.contains(mousePos) ? sf::Color(255, 210, 60) : sf::Color(255, 191, 0));
        
        if (currentScreen == Screen::Blackjack) 
        {
            blackjackScreen.updateHover(mousePos);
        }
        if (currentScreen == Screen::Crash) 
        {
            crashScreen.updateHover(mousePos);
        }

        window.clear(sf::Color::Black);

        if (currentScreen == Screen::MainMenu) 
        {
            window.draw(title);
            window.draw(subtitle);
            window.draw(balanceBox);
            window.draw(balanceText);

            blackjackCard.draw(window);
            slotsCard.draw(window);
            crashCard.draw(window);

            blackjackPlay.draw(window);
            slotsPlay.draw(window);
            crashPlay.draw(window);
            exitButton.draw(window);

            if (!statusText.getString().isEmpty()) 
            {
                window.draw(statusText);
            }
        } 
        else if (currentScreen == Screen::Slots) 
        {
            {
                constexpr float screenCx = 640.f;
                sf::FloatRect titleB = slotsTitle.getLocalBounds();
                slotsTitle.setOrigin(
                    titleB.left + titleB.width * 0.5f,
                    titleB.top + titleB.height * 0.5f);
                slotsTitle.setPosition(screenCx, 52.f);
                sf::FloatRect subB = slotsSubheading.getLocalBounds();
                slotsSubheading.setOrigin(
                    subB.left + subB.width * 0.5f,
                    subB.top + subB.height * 0.5f);
                slotsSubheading.setPosition(screenCx, 96.f);
            }
            window.draw(slotsTitle);
            window.draw(slotsSubheading);
            window.draw(balanceBox);
            window.draw(balanceText);
            backButton.draw(window);

            window.draw(slotsPanel);
            window.draw(messagePanel);
            window.draw(payoutPanel);
            {
                const sf::FloatRect pb = payoutPanel.getGlobalBounds();
                const sf::FloatRect tBounds = payoutTitle.getLocalBounds();
                payoutTitle.setOrigin(
                    tBounds.left + tBounds.width * 0.5f,
                    tBounds.top + tBounds.height * 0.5f);
                payoutTitle.setPosition(pb.left + pb.width * 0.5f, pb.top + 32.f);
            }
            window.draw(payoutTitle);
            drawPayoutTable(window, font, payoutPanel, slotsGame.currentBet());
            window.draw(slotsMessageText);

            window.draw(reel1);
            window.draw(reel2);
            window.draw(reel3);
            window.draw(reelInner1);
            window.draw(reelInner2);
            window.draw(reelInner3);
            window.draw(reelLabel1);
            window.draw(reelLabel2);
            window.draw(reelLabel3);
            drawSlotSymbol(window, reelA, reelInner1, font);
            drawSlotSymbol(window, reelB, reelInner2, font);
            drawSlotSymbol(window, reelC, reelInner3, font);

            betMinus.draw(window);
            window.draw(betBox);
            window.draw(betText);
            betPlus.draw(window);
            spinButton.draw(window);
        } 
        else if (currentScreen == Screen::Blackjack) 
        {
            window.draw(balanceBox);
            window.draw(balanceText);
            backButton.draw(window);
            blackjackScreen.draw(window);
        } 
        else if (currentScreen == Screen::Crash) 
        {
            window.draw(balanceBox);
            window.draw(balanceText);
            backButton.draw(window);
            crashScreen.draw(window);
        }

        window.display();
    }

    return 0;
}