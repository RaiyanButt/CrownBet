#include <iostream>
#include <string>
#include <random>
#include <optional>
#include <limits>
#include <vector>
#include <algorithm>

/**
 * @brief Clears the current input error state and discards the rest of the line.
 *
 * This is used after input operations to recover from invalid input and to
 * remove any leftover characters from the input buffer.
 */
static void clearInputLine()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * @brief Prompts the user to enter an integer.
 *
 * Repeats until a valid integer is entered.
 *
 * @param prompt The message shown to the user before reading input.
 * @return The validated integer entered by the user.
 */
static int readInt(const std::string& prompt) 
{
    while (true) 
    {
        std::cout << prompt;
        int x;

        if (std::cin >> x) 
        {
            clearInputLine();
            return x;
        }

        clearInputLine();
        std::cout << "Invalid input. Please enter an integer.\n";
    }
}

/**
 * @brief Random number generator wrapper for integer and floating-point values.
 *
 * Uses a Mersenne Twister engine and supports either a user-provided seed or
 * a random seed from std::random_device.
 */
class RNG 
{
public:
    /**
     * @brief Constructs the random number generator.
     *
     * @param seed Optional seed value. If not provided, a random seed is used.
     */
    explicit RNG(std::optional<unsigned int> seed = std::nullopt) 
    {
        if (seed.has_value()) 
        {
            eng.seed(*seed);
        } 
        else 
        {
            std::random_device rd;
            eng.seed(rd());
        }
    }

    /**
     * @brief Generates a uniformly distributed integer in a closed range.
     *
     * @param lo Lower bound of the range.
     * @param hi Upper bound of the range.
     * @return A random integer between lo and hi inclusive.
     */
    int uniformInt(int lo, int hi) 
    {
        std::uniform_int_distribution<int> dist(lo, hi);
        return dist(eng);
    }

    /**
     * @brief Generates a uniformly distributed floating-point number.
     *
     * @param lo Lower bound of the range.
     * @param hi Upper bound of the range.
     * @return A random double in the range [lo, hi].
     */
    double uniformDouble(double lo, double hi) 
    {
        std::uniform_real_distribution<double> dist(lo, hi);
        return dist(eng);
    }

private:
    /// @brief Underlying Mersenne Twister random engine.
    std::mt19937 eng;
};

/**
 * @brief Represents the player's coin balance.
 *
 * Provides methods for checking available funds, debiting bets,
 * and crediting winnings.
 */
class Wallet 
{
public:
    /**
     * @brief Constructs a wallet with an initial number of coins.
     *
     * @param startingCoins Initial wallet balance.
     */
    explicit Wallet(int startingCoins) : coins_(startingCoins) {}

    /**
     * @brief Gets the current wallet balance.
     *
     * @return Current number of coins.
     */
    int balance() const 
    { 
        return coins_; 
    }

    /**
     * @brief Checks whether a bet amount is valid.
     *
     * A valid bet must be greater than 0 and no more than the current balance.
     *
     * @param amount Bet amount to validate.
     * @return true if the bet is valid, false otherwise.
     */
    bool canBet(int amount) const 
    {
        return amount > 0 && amount <= coins_;
    }

    /**
     * @brief Removes coins from the wallet for a bet.
     *
     * @param amount Number of coins to deduct.
     * @return true if the debit succeeded, false if the amount was invalid.
     */
    bool debit(int amount) 
    {
        if (!canBet(amount)) return false;
        coins_ -= amount;
        return true;
    }

    /**
     * @brief Adds coins to the wallet.
     *
     * @param amount Number of coins to add.
     */
    void credit(int amount) 
    {
        if (amount > 0) coins_ += amount;
    }

private:
    /// @brief Current number of coins in the wallet.
    int coins_;
};

/**
 * @brief Converts a slot symbol ID into its display name.
 *
 * @param s Symbol identifier.
 * @return Name of the symbol as a string.
 */
static std::string symbolName(int s) 
{
    switch (s) 
    {
        case 0: return "Cherry";
        case 1: return "Lemon";
        case 2: return "Bell";
        case 3: return "Bar";
        case 4: return "Seven";
        default: return "?";
    }
}

/**
 * @brief Displays the instructions for the slot machine game.
 */
static void printSlotsInstructions() 
{
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

/**
 * @brief Displays the instructions for the blackjack game.
 */
static void printBlackjackInstructions() 
{
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

/**
 * @brief Displays the instructions for the rocket crash game.
 */
static void printCrashInstructions() 
{
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

/**
 * @brief Prompts the user to place a valid bet.
 *
 * Displays the current balance and repeatedly asks for input until the user
 * enters a valid bet or 0 to cancel.
 *
 * @param wallet Reference to the player's wallet.
 * @return The validated bet amount, or 0 if cancelled.
 */
static int promptBet(Wallet& wallet) 
{
    std::cout << "\nBalance: " << wallet.balance() << " coins\n";
    std::cout << "Enter bet amount (0 to cancel).\n";

    while (true) 
    {
        int bet = readInt("Bet: ");
        if (bet == 0) return 0;
        if (wallet.canBet(bet)) return bet;
        std::cout << "Invalid bet. Must be between 1 and " << wallet.balance() << ".\n";
    }
}

/**
 * @brief Generates a slot machine symbol using weighted probabilities.
 *
 * Symbol mapping:
 * - 0 = Cherry
 * - 1 = Lemon
 * - 2 = Bell
 * - 3 = Bar
 * - 4 = Seven
 *
 * @param rng Random number generator.
 * @return Generated symbol identifier.
 */
static int spinSymbol(RNG& rng) 
{
    int roll = rng.uniformInt(1, 100);
    if (roll <= 40) return 0;
    if (roll <= 65) return 1;
    if (roll <= 83) return 2;
    if (roll <= 95) return 3;
    return 4;
}

/**
 * @brief Computes the payout multiplier for a slot result.
 *
 * @param a First slot symbol.
 * @param b Second slot symbol.
 * @param c Third slot symbol.
 * @return Multiplier applied to the player's bet.
 */
static int computeMultiplier(int a, int b, int c) 
{
    if (a == 4 && b == 4 && c == 4) return 10;
    if (a == 3 && b == 3 && c == 3) return 6;
    if (a == 2 && b == 2 && c == 2) return 4;
    if (a == 1 && b == 1 && c == 1) return 3;
    if (a == 0 && b == 0 && c == 0) return 3;

    int cherries = (a == 0) + (b == 0) + (c == 0);

    if (cherries >= 2) return 2;

    return 0;
}

/**
 * @brief Runs one round of the slot machine game.
 *
 * Prompts for a bet, spins three symbols, computes the payout,
 * updates the wallet, and displays the result.
 *
 * @param wallet Reference to the player's wallet.
 * @param rng Random number generator.
 */
static void playSlots(Wallet& wallet, RNG& rng) 
{
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

/**
 * @brief Runs one round of the simplified blackjack game.
 *
 * The player and dealer draw initial cards. The player may hit or stand,
 * and then the dealer draws until reaching at least 17. The outcome is
 * used to update the wallet balance.
 *
 * @param wallet Reference to the player's wallet.
 * @param rng Random number generator.
 */
static void playBlackjack(Wallet& wallet, RNG& rng) 
{
    int bet = promptBet(wallet);

    if (bet == 0) return;

    wallet.debit(bet);
    auto drawCard = [&rng]() { return rng.uniformInt(1, 11); };
    int playerTotal = drawCard() + drawCard();
    int dealerTotal = drawCard() + drawCard();
    std::cout << "\nYour initial total: " << playerTotal << "\n";
    std::string action;

    while (playerTotal < 21) 
    {
        std::cout << "Hit or stand? (h/s): ";
        std::getline(std::cin, action);

        if (action == "h") playerTotal += drawCard();
        else break;

        std::cout << "Your total: " << playerTotal << "\n";
    }
    while (dealerTotal < 17) dealerTotal += drawCard();
    std::cout << "Dealer total: " << dealerTotal << "\n";

    if ((playerTotal > 21) || (dealerTotal <= 21 && dealerTotal > playerTotal)) 
    {
        std::cout << "You lose!\n";
    } 
    else if (playerTotal == dealerTotal) 
    {
        std::cout << "Push! Bet returned.\n";
        wallet.credit(bet);
    } 
    else 
    {
        std::cout << "You win!\n";
        wallet.credit(bet * 2);
    }
    std::cout << "New balance: " << wallet.balance() << " coins\n";
}

/**
 * @brief Runs one round of the rocket crash game.
 *
 * The player places a bet and waits while the multiplier increases.
 * The player may cash out before the rocket crashes to receive a payout.
 *
 * @param wallet Reference to the player's wallet.
 * @param rng Random number generator.
 */
static void playCrash(Wallet& wallet, RNG& rng) 
{
    int bet = promptBet(wallet);
    if (bet == 0) return;
    wallet.debit(bet);
    double multiplier = 1.0;
    double crashPoint = rng.uniformDouble(1.5, 10.0);
    std::string input;
    std::cout << "\nMultiplier starts at 1.00x\n";

    while (true) 
    {
        std::cout << "Press Enter to cash out or wait for multiplier to increase...";
        std::getline(std::cin, input);

        if (!input.empty() || multiplier >= crashPoint) break;

        multiplier += 0.5;
        std::cout << "Multiplier: " << multiplier << "x\n";
    }
    if (multiplier >= crashPoint) 
    {
        std::cout << "Rocket crashed at " << crashPoint << "x! You lose your bet.\n";
    } 
    else 
    {
        int payout = static_cast<int>(bet * multiplier);
        wallet.credit(payout);
        std::cout << "You cashed out at " << multiplier << "x! You win " << payout << " coins.\n";
    }
    std::cout << "New balance: " << wallet.balance() << " coins\n";
}

/**
 * @brief Displays the main casino lobby menu.
 *
 * @param wallet Reference to the player's wallet for showing current balance.
 */
static void printLobby(const Wallet& wallet) 
{
    std::cout << "\n=============================\n";
    std::cout << "      CS3307 Casino (CLI)\n";
    std::cout << "=============================\n";
    std::cout << "Balance: " << wallet.balance() << " coins\n";
    std::cout << "1) Play Slots\n2) Slots Instructions\n";
    std::cout << "3) Play Blackjack\n4) Blackjack Instructions\n";
    std::cout << "5) Play Rocket Crash\n6) Rocket Crash Instructions\n";
    std::cout << "7) Quit\n";
}

/**
 * @brief Entry point of the casino application.
 *
 * Supports an optional command-line seed:
 * @code
 * ./program --seed 1234
 * @endcode
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return Exit status code.
 */
int main(int argc, char** argv) 
{
    std::optional<unsigned int> seed;

    if (argc == 3 && std::string(argv[1]) == "--seed") 
    {
        try 
        { 
            seed = static_cast<unsigned int>(std::stoul(argv[2])); 
        }
        catch (...) { std::cout << "Invalid seed. Using random seed instead.\n"; }
    }
    RNG rng(seed);
    Wallet wallet(1000);

    while (true) 
    {
        if (wallet.balance() <= 0) 
        { 
            std::cout << "\nYou are out of coins. Game over.\n"; break; 
        }
        
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