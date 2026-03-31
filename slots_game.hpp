#pragma once

#include "rng.hpp"
#include "wallet.hpp"

struct SlotsResult {
    int reelA = 0;
    int reelB = 1;
    int reelC = 2;
    int multiplier = 0;
    int payout = 0;
    bool won = false;
};

class SlotsGame {
public:
    explicit SlotsGame(int startingBet = 50);

    int currentBet() const;
    void increaseBet(const Wallet& wallet);
    void decreaseBet();
    SlotsResult spin(Wallet& wallet, RNG& rng);

private:
    static int spinSymbol(RNG& rng);
    static int computeMultiplier(int a, int b, int c);

    int currentBet_;
};
