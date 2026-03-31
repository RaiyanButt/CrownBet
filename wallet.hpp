#pragma once

class Wallet {
public:
    explicit Wallet(int startingCoins) : coins_(startingCoins) {}

    int balance() const
    {
        return coins_;
    }

    bool canBet(int amount) const
    {
        return amount > 0 && amount <= coins_;
    }

    bool debit(int amount)
    {
        if (!canBet(amount)) return false;
        coins_ -= amount;
        return true;
    }

    void credit(int amount)
    {
        if (amount > 0) coins_ += amount;
    }

private:
    int coins_;
};
