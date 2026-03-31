#pragma once

#include "rng.hpp"
#include "wallet.hpp"

enum class CrashPhase {
    Idle,
    Flying,
    RoundOver
};

enum class CrashOutcome {
    None,
    CashedOut,
    Crashed
};

class CrashGame {
public:
    explicit CrashGame(int startingBet = 50);

    int currentBet() const;
    CrashPhase phase() const;
    CrashOutcome lastOutcome() const;
    double multiplier() const;
    double crashPoint() const;
    int roundBet() const;
    int lastPayout() const;

    void increaseBet(const Wallet& wallet);
    void decreaseBet();

    bool startRound(Wallet& wallet, RNG& rng);
    void cashOut(Wallet& wallet);
    void tick();
    void acknowledgeRound();
    void clampBetToWallet(const Wallet& wallet);

private:
    int currentBet_;
    int roundBet_ = 0;
    double multiplier_ = 1.0;
    double crashPoint_ = 1.0;
    int tickCount_ = 0;
    CrashPhase phase_ = CrashPhase::Idle;
    CrashOutcome lastOutcome_ = CrashOutcome::None;
    int lastPayout_ = 0;
};
