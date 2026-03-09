#include <iostream>
#include <string>
#include <random>
#include <optional>
#include <limits>
#include <vector>
#include <algorithm>

static void clearInputLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int x;
        if (std::cin >> x) {
            clearInputLine();
            return x;
        }
        clearInputLine();
        std::cout << "Invalid input. Please enter an integer.\n";
    }
}

class RNG {
public:
    explicit RNG(std::optional<unsigned int> seed = std::nullopt) {
        if (seed.has_value()) {
            eng.seed(*seed);
        } else {
            std::random_device rd;
            eng.seed(rd());
        }
    }

    int uniformInt(int lo, int hi) {
        std::uniform_int_distribution<int> dist(lo, hi);
        return dist(eng);
    }

    double uniformDouble(double lo, double hi) {
        std::uniform_real_distribution<double> dist(lo, hi);
        return dist(eng);
    }

private:
    std::mt19937 eng;
};

class Wallet {
public:
    explicit Wallet(int startingCoins) : coins_(startingCoins) {}

    int balance() const { return coins_; }

    bool canBet(int amount) const {
        return amount > 0 && amount <= coins_;
    }

    bool debit(int amount) {
        if (!canBet(amount)) return false;
        coins_ -= amount;
        return true;
    }

    void credit(int amount) {
        if (amount > 0) coins_ += amount;
    }

private:
    int coins_;
};

static std::string symbolName(int s) {
    switch (s) {
        case 0: return "Cherry";
        case 1: return "Lemon";
        case 2: return "Bell";
        case 3: return "Bar";
        case 4: return "Seven";
        default: return "?";
    }
}

static void printSlotsInstructions() {
    std::cout << "\n--- Slots Instructions ---\n";
    std::cout << "Welcome to the Slot Machine!\n\n";
    std::cout << "How to play:\n";
    std::cout << "1. You will see your current coin balance before each spin.\n";
    std::cout << "2. Enter the amount of coins you want to bet.\n";
    std::cout << "   - You cannot bet more than your current balance.\n";
    std::cout << "   - Enter 0 to cancel and return to the lobby.\n";
    std::cout << "3. The slot machine will spin three reels and display symbols.\n\n";
    std::cout << "Possible symbols:\n";
    std::cout << "Cherry, Lemon, Bell, Bar, Seven\n\n";
    std::cout << "Winning combinations & payouts:\n";
    std::cout << "Three Sevens            -> 10x your bet\n";
    std::cout << "Three Bars              -> 6x your bet\n";
    std::cout << "Three Bells             -> 4x your bet\n";
    std::cout << "Three Lemons            -> 3x your bet\n";
    std::cout << "Three Cherries          -> 3x your bet\n";
    std::cout << "Any two or more Cherries -> 2x your bet\n";
    std::cout << "No match                -> you lose your bet\n\n";
    std::cout << "Tips:\n";
    std::cout << "- Cherries are lucky! Even two can earn you a small payout.\n";
    std::cout << "- Always check your balance before betting.\n\n";
    std::cout << "Press Enter to return to the lobby...";
    std::cin.get();
}

static void printBlackjackInstructions() {
    std::cout << "\n--- Blackjack Instructions ---\n";
    std::cout << "Blackjack is a card game where the goal is to reach 21 without going over.\n";
    std::cout << "You will play against a dealer.\n\n";
    std::cout << "Basic rules:\n";
    std::cout << "- You place a bet before the round starts.\n";
    std::cout << "- You and the dealer receive cards.\n";
    std::cout << "- You may choose to hit (draw another card) or stand.\n";
    std::cout << "- If your total exceeds 21, you bust and lose the bet.\n";
    std::cout << "- The dealer then plays according to fixed rules.\n";
    std::cout << "- Whoever is closer to 21 wins the round.\n\n";
    std::cout << "Press Enter to return to the lobby...";
    std::cin.get();
}

static void printCrashInstructions() {
    std::cout << "\n--- Rocket Crash Instructions ---\n";
    std::cout << "In the crash game, a multiplier increases over time.\n\n";
    std::cout << "Basic rules:\n";
    std::cout << "- You place a bet before the round begins.\n";
    std::cout << "- A multiplier begins increasing from 1.00x.\n";
    std::cout << "- You may cash out at any time.\n";
    std::cout << "- Your winnings equal bet × multiplier when you cash out.\n";
    std::cout << "- However, the rocket can crash randomly at any time.\n";
    std::cout << "- If it crashes before you cash out, you lose your bet.\n\n";
    std::cout << "Press Enter to return to the lobby...";
    std::cin.get();
}

static int promptBet(Wallet& wallet) {
    std::cout << "\nBalance: " << wallet.balance() << " coins\n";
    std::cout << "Enter bet amount (0 to cancel).\n";
    while (true) {
        int bet = readInt("Bet: ");
        if (bet == 0) return 0;
        if (wallet.canBet(bet)) return bet;
        std::cout << "Invalid bet. Must be between 1 and " << wallet.balance() << ".\n";
    }
}

static int spinSymbol(RNG& rng) {
    int roll = rng.uniformInt(1, 100);
    if (roll <= 40) return 0;
    if (roll <= 65) return 1;
    if (roll <= 83) return 2;
    if (roll <= 95) return 3;
    return 4;
}

static int computeMultiplier(int a, int b, int c) {
    if (a == 4 && b == 4 && c == 4) return 10;
    if (a == 3 && b == 3 && c == 3) return 6;
    if (a == 2 && b == 2 && c == 2) return 4;
    if (a == 1 && b == 1 && c == 1) return 3;
    if (a == 0 && b == 0 && c == 0) return 3;
    int cherries = (a == 0) + (b == 0) + (c == 0);
    if (cherries >= 2) return 2;
    return 0;
}

static void playSlots(Wallet& wallet, RNG& rng) {
    int bet = promptBet(wallet);
    if (bet == 0) return;
    wallet.debit(bet);
    int a = spinSymbol(rng);
    int b = spinSymbol(rng);
    int c = spinSymbol(rng);
    std::cout << "\n[" << symbolName(a) << "] [" << symbolName(b) << "] [" << symbolName(c) << "]\n";
    int mult = computeMultiplier(a, b, c);
    int payout = bet * mult;
    if (payout > 0) wallet.credit(payout);
    std::cout << (payout > 0 ? "You win " : "No win this time.") << payout << " coins!\n";
    std::cout << "New balance: " << wallet.balance() << " coins\n";
}

static void playBlackjack(Wallet& wallet, RNG& rng) {
    int bet = promptBet(wallet);
    if (bet == 0) return;
    wallet.debit(bet);
    auto drawCard = [&rng]() { return rng.uniformInt(1, 11); };
    int playerTotal = drawCard() + drawCard();
    int dealerTotal = drawCard() + drawCard();
    std::cout << "\nYour initial total: " << playerTotal << "\n";
    std::string action;
    while (playerTotal < 21) {
        std::cout << "Hit or stand? (h/s): ";
        std::getline(std::cin, action);
        if (action == "h") playerTotal += drawCard();
        else break;
        std::cout << "Your total: " << playerTotal << "\n";
    }
    while (dealerTotal < 17) dealerTotal += drawCard();
    std::cout << "Dealer total: " << dealerTotal << "\n";
    if ((playerTotal > 21) || (dealerTotal <= 21 && dealerTotal > playerTotal)) {
        std::cout << "You lose!\n";
    } else if (playerTotal == dealerTotal) {
        std::cout << "Push! Bet returned.\n";
        wallet.credit(bet);
    } else {
        std::cout << "You win!\n";
        wallet.credit(bet * 2);
    }
    std::cout << "New balance: " << wallet.balance() << " coins\n";
}

static void playCrash(Wallet& wallet, RNG& rng) {
    int bet = promptBet(wallet);
    if (bet == 0) return;
    wallet.debit(bet);
    double multiplier = 1.0;
    double crashPoint = rng.uniformDouble(1.5, 10.0);
    std::string input;
    std::cout << "\nMultiplier starts at 1.00x\n";
    while (true) {
        std::cout << "Press Enter to cash out or wait for multiplier to increase...";
        std::getline(std::cin, input);
        if (!input.empty() || multiplier >= crashPoint) break;
        multiplier += 0.5;
        std::cout << "Multiplier: " << multiplier << "x\n";
    }
    if (multiplier >= crashPoint) {
        std::cout << "Rocket crashed at " << crashPoint << "x! You lose your bet.\n";
    } else {
        int payout = static_cast<int>(bet * multiplier);
        wallet.credit(payout);
        std::cout << "You cashed out at " << multiplier << "x! You win " << payout << " coins.\n";
    }
    std::cout << "New balance: " << wallet.balance() << " coins\n";
}

static void printLobby(const Wallet& wallet) {
    std::cout << "\n=============================\n";
    std::cout << "      CS3307 Casino (CLI)\n";
    std::cout << "=============================\n";
    std::cout << "Balance: " << wallet.balance() << " coins\n";
    std::cout << "1) Play Slots\n2) Slots Instructions\n";
    std::cout << "3) Play Blackjack\n4) Blackjack Instructions\n";
    std::cout << "5) Play Rocket Crash\n6) Rocket Crash Instructions\n";
    std::cout << "7) Quit\n";
}

int main(int argc, char** argv) {
    std::optional<unsigned int> seed;
    if (argc == 3 && std::string(argv[1]) == "--seed") {
        try { seed = static_cast<unsigned int>(std::stoul(argv[2])); }
        catch (...) { std::cout << "Invalid seed. Using random seed instead.\n"; }
    }
    RNG rng(seed);
    Wallet wallet(1000);
    while (true) {
        if (wallet.balance() <= 0) { std::cout << "\nYou are out of coins. Game over.\n"; break; }
        printLobby(wallet);
        int choice = readInt("Choose an option: ");
        if (choice == 1) playSlots(wallet, rng);
        else if (choice == 2) printSlotsInstructions();
        else if (choice == 3) playBlackjack(wallet, rng);
        else if (choice == 4) printBlackjackInstructions();
        else if (choice == 5) playCrash(wallet, rng);
        else if (choice == 6) printCrashInstructions();
        else if (choice == 7) { std::cout << "Thanks for playing!\n"; break; }
        else std::cout << "Invalid option.\n";
    }
    return 0;
}