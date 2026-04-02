#pragma once

#include <vector>

#include "rng.hpp"
#include "wallet.hpp"

enum class BlackjackPhase {
    Idle,
    PlayerTurn,
    RoundOver
};

enum class BlackjackOutcome {
    None,
    Win,
    Lose,
    Push
};

struct BlackjackCard {
    bool isAce = false;
    /// 2-10 when not an ace (10 includes 10/J/Q/K); unused when isAce.
    int value = 0;
};

class BlackjackGame {
public:
    explicit BlackjackGame(int startingBet = 50);

    int currentBet() const;
    BlackjackPhase phase() const;
    BlackjackOutcome outcome() const;
    int roundBet() const;
    /// Coins returned on last resolution (win/push); 0 on loss.
    int lastPayout() const;
    bool lastWinWasBlackjack() const;

    const std::vector<BlackjackCard>& playerCards() const;
    const std::vector<BlackjackCard>& dealerCards() const;
    bool dealerHoleHidden() const;
    int playerTotal() const;
    int dealerTotal() const;

    void increaseBet(const Wallet& wallet);
    void decreaseBet();
    void clampBetToWallet(const Wallet& wallet);

    bool deal(Wallet& wallet, RNG& rng);
    void hit(Wallet& wallet, RNG& rng);
    void stand(Wallet& wallet, RNG& rng);
    void newHand();

private:
    static int handTotal(const std::vector<BlackjackCard>& hand);
    static bool isNatural(const std::vector<BlackjackCard>& hand);
    static BlackjackCard drawCard(RNG& rng);
    void resolveAgainstDealer(Wallet& wallet);

    int currentBet_;
    int roundBet_ = 0;
    BlackjackPhase phase_ = BlackjackPhase::Idle;
    BlackjackOutcome outcome_ = BlackjackOutcome::None;
    int lastPayout_ = 0;
    bool lastWinByBlackjack_ = false;
    std::vector<BlackjackCard> player_;
    std::vector<BlackjackCard> dealer_;
};
