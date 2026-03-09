#include <iostream>
#include <string>
#include <random>
#include <optional>
#include <limits>

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

static int promptBet(Wallet& wallet) {
    std::cout << "\n--- Slots ---\n";
    std::cout << "Balance: " << wallet.balance() << " coins\n";
    std::cout << "Enter bet amount (0 to cancel).\n";

    while (true) {
        int bet = readInt("Bet: ");
        if (bet == 0) return 0;
        if (wallet.canBet(bet)) return bet;
        std::cout << "Invalid bet. Must be between 1 and " << wallet.balance() << ".\n";
    }
}

static int spinSymbol(RNG& rng) {
    // Weighted probabilities (total 100):
    // Cherry 40, Lemon 25, Bell 18, Bar 12, Seven 5
    int roll = rng.uniformInt(1, 100);
    if (roll <= 40) return 0;
    if (roll <= 65) return 1;
    if (roll <= 83) return 2;
    if (roll <= 95) return 3;
    return 4;
}

static int computeMultiplier(int a, int b, int c) {
    // 3 Sevens  = 10x
    // 3 Bars    =  6x
    // 3 Bells   =  4x
    // 3 Lemons  =  3x
    // 3 Cherries=  3x
    // Any 2+ Cherries = 2x
    // Else 0
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
    if (bet == 0) {
        std::cout << "Slots cancelled.\n";
        return;
    }

    // Deduct bet up front
    wallet.debit(bet);

    int a = spinSymbol(rng);
    int b = spinSymbol(rng);
    int c = spinSymbol(rng);

    std::cout << "\n--- Slots Spin ---\n";
    std::cout << "[" << symbolName(a) << "] [" << symbolName(b) << "] [" << symbolName(c) << "]\n";

    int mult = computeMultiplier(a, b, c);
    int payout = bet * mult;

    if (payout > 0) {
        wallet.credit(payout);
        std::cout << "You win " << payout << " coins! (x" << mult << ")\n";
    } else {
        std::cout << "No win this time.\n";
    }

    std::cout << "New balance: " << wallet.balance() << " coins\n";
}

static void printLobby(const Wallet& wallet) {
    std::cout << "\n=============================\n";
    std::cout << "      CS3307 Casino (CLI)\n";
    std::cout << "=============================\n";
    std::cout << "Balance: " << wallet.balance() << " coins\n";
    std::cout << "1) Slots\n";
    std::cout << "2) Quit\n";
}

int main(int argc, char** argv) {
    std::optional<unsigned int> seed;
    if (argc == 3 && std::string(argv[1]) == "--seed") {
        try {
            seed = static_cast<unsigned int>(std::stoul(argv[2]));
        } catch (...) {
            std::cout << "Invalid seed. Using random seed instead.\n";
        }
    }

    RNG rng(seed);
    Wallet wallet(1000);

    while (true) {
        if (wallet.balance() <= 0) {
            std::cout << "\nYou are out of coins. Game over.\n";
            break;
        }

        printLobby(wallet);
        int choice = readInt("Choose an option: ");

        if (choice == 1) {
            playSlots(wallet, rng);
        } else if (choice == 2) {
            std::cout << "Thanks for playing!\n";
            break;
        } else {
            std::cout << "Invalid option.\n";
        }
    }

    return 0;
}