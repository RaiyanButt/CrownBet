#include "blackjack_game.hpp"

BlackjackGame::BlackjackGame(int startingBet)
    : currentBet_(startingBet)
{
}

int BlackjackGame::currentBet() const
{
    return currentBet_;
}

BlackjackPhase BlackjackGame::phase() const
{
    return phase_;
}

BlackjackOutcome BlackjackGame::outcome() const
{
    return outcome_;
}

int BlackjackGame::roundBet() const
{
    return roundBet_;
}

int BlackjackGame::lastPayout() const
{
    return lastPayout_;
}

bool BlackjackGame::lastWinWasBlackjack() const
{
    return lastWinByBlackjack_;
}

const std::vector<BlackjackCard>& BlackjackGame::playerCards() const
{
    return player_;
}

const std::vector<BlackjackCard>& BlackjackGame::dealerCards() const
{
    return dealer_;
}

bool BlackjackGame::dealerHoleHidden() const
{
    return phase_ == BlackjackPhase::PlayerTurn && dealer_.size() >= 2;
}

int BlackjackGame::playerTotal() const
{
    return handTotal(player_);
}

int BlackjackGame::dealerTotal() const
{
    return handTotal(dealer_);
}

void BlackjackGame::increaseBet(const Wallet& wallet)
{
    if (phase_ != BlackjackPhase::Idle) return;
    if (currentBet_ < wallet.balance()) {
        currentBet_ += 10;
    }
}

void BlackjackGame::decreaseBet()
{
    if (phase_ != BlackjackPhase::Idle) return;
    if (currentBet_ > 10) {
        currentBet_ -= 10;
    }
}

void BlackjackGame::clampBetToWallet(const Wallet& wallet)
{
    if (currentBet_ > wallet.balance()) {
        currentBet_ = wallet.balance();
    }
    if (currentBet_ < 10) {
        currentBet_ = 10;
    }
    if (wallet.balance() < 10) {
        currentBet_ = wallet.balance();
    }
}

int BlackjackGame::handTotal(const std::vector<BlackjackCard>& hand)
{
    int sum = 0;
    int aces = 0;
    for (const BlackjackCard& c : hand) {
        if (c.isAce) {
            ++aces;
        } else {
            sum += c.value;
        }
    }
    sum += aces * 11;
    while (sum > 21 && aces > 0) {
        sum -= 10;
        --aces;
    }
    return sum;
}

bool BlackjackGame::isNatural(const std::vector<BlackjackCard>& hand)
{
    return hand.size() == 2 && handTotal(hand) == 21;
}

BlackjackCard BlackjackGame::drawCard(RNG& rng)
{
    const int r = rng.uniformInt(1, 13);
    if (r == 1) {
        return BlackjackCard{true, 0};
    }
    if (r >= 10) {
        return BlackjackCard{false, 10};
    }
    return BlackjackCard{false, r};
}

bool BlackjackGame::deal(Wallet& wallet, RNG& rng)
{
    if (phase_ != BlackjackPhase::Idle) {
        return false;
    }
    if (!wallet.canBet(currentBet_)) {
        return false;
    }

    wallet.debit(currentBet_);
    roundBet_ = currentBet_;
    player_.clear();
    dealer_.clear();
    outcome_ = BlackjackOutcome::None;
    lastPayout_ = 0;
    lastWinByBlackjack_ = false;

    player_.push_back(drawCard(rng));
    player_.push_back(drawCard(rng));
    dealer_.push_back(drawCard(rng));
    dealer_.push_back(drawCard(rng));

    phase_ = BlackjackPhase::PlayerTurn;
    if (handTotal(player_) == 21) {
        stand(wallet, rng);
    }
    return true;
}

void BlackjackGame::hit(Wallet& wallet, RNG& rng)
{
    if (phase_ != BlackjackPhase::PlayerTurn) {
        return;
    }

    player_.push_back(drawCard(rng));
    const int t = handTotal(player_);
    if (t > 21) {
        lastPayout_ = 0;
        lastWinByBlackjack_ = false;
        outcome_ = BlackjackOutcome::Lose;
        phase_ = BlackjackPhase::RoundOver;
    } else if (t == 21) {
        stand(wallet, rng);
    }
}

void BlackjackGame::resolveAgainstDealer(Wallet& wallet)
{
    const int pt = handTotal(player_);
    const int dt = handTotal(dealer_);
    const bool playerNatural = isNatural(player_);
    const bool dealerNatural = isNatural(dealer_);

    lastPayout_ = 0;
    lastWinByBlackjack_ = false;

    if (pt > 21) {
        outcome_ = BlackjackOutcome::Lose;
        return;
    }

    if (playerNatural && dealerNatural) {
        outcome_ = BlackjackOutcome::Push;
        wallet.credit(roundBet_);
        lastPayout_ = roundBet_;
        return;
    }
    if (playerNatural) {
        outcome_ = BlackjackOutcome::Win;
        wallet.credit(roundBet_ * 3);
        lastPayout_ = roundBet_ * 3;
        lastWinByBlackjack_ = true;
        return;
    }
    if (dealerNatural) {
        outcome_ = BlackjackOutcome::Lose;
        return;
    }
    if (dt > 21) {
        outcome_ = BlackjackOutcome::Win;
        wallet.credit(roundBet_ * 2);
        lastPayout_ = roundBet_ * 2;
        return;
    }
    if (dt > pt) {
        outcome_ = BlackjackOutcome::Lose;
        return;
    }
    if (pt > dt) {
        outcome_ = BlackjackOutcome::Win;
        wallet.credit(roundBet_ * 2);
        lastPayout_ = roundBet_ * 2;
        return;
    }

    outcome_ = BlackjackOutcome::Push;
    wallet.credit(roundBet_);
    lastPayout_ = roundBet_;
}

void BlackjackGame::stand(Wallet& wallet, RNG& rng)
{
    if (phase_ != BlackjackPhase::PlayerTurn) {
        return;
    }

    while (handTotal(dealer_) < 17) {
        dealer_.push_back(drawCard(rng));
    }

    resolveAgainstDealer(wallet);
    phase_ = BlackjackPhase::RoundOver;
}

void BlackjackGame::newHand()
{
    phase_ = BlackjackPhase::Idle;
    player_.clear();
    dealer_.clear();
    outcome_ = BlackjackOutcome::None;
    roundBet_ = 0;
    lastPayout_ = 0;
    lastWinByBlackjack_ = false;
}
