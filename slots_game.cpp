#include "slots_game.hpp"

SlotsGame::SlotsGame(int startingBet)
    : currentBet_(startingBet)
{
}

int SlotsGame::currentBet() const
{
    return currentBet_;
}

void SlotsGame::increaseBet(const Wallet& wallet)
{
    if (currentBet_ + 10 <= wallet.balance()) {
        currentBet_ += 10;
    }
}

void SlotsGame::decreaseBet()
{
    if (currentBet_ > 10) {
        currentBet_ -= 10;
    }
}

SlotsResult SlotsGame::spin(Wallet& wallet, RNG& rng)
{
    SlotsResult result;

    if (!wallet.canBet(currentBet_)) {
        return result;
    }

    wallet.debit(currentBet_);

    result.reelA = spinSymbol(rng);
    result.reelB = spinSymbol(rng);
    result.reelC = spinSymbol(rng);
    result.multiplier = computeMultiplier(result.reelA, result.reelB, result.reelC);
    result.payout = currentBet_ * result.multiplier;
    result.won = result.payout > 0;

    if (result.won) {
        wallet.credit(result.payout);
    }

    if (wallet.balance() > 0 && currentBet_ > wallet.balance()) {
        currentBet_ = wallet.balance();
    }

    return result;
}

int SlotsGame::spinSymbol(RNG& rng)
{
    int roll = rng.uniformInt(1, 100);
    if (roll <= 40) return 0;
    if (roll <= 65) return 1;
    if (roll <= 83) return 2;
    if (roll <= 95) return 3;
    return 4;
}

int SlotsGame::computeMultiplier(int a, int b, int c)
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
