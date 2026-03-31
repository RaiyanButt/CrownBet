#include "crash_game.hpp"

#include <algorithm>
#include <cmath>

namespace {

constexpr int kTicksToDouble = 11;
constexpr double kGrowthBeta = 0.11;

double growthDenom()
{
    double d = 0.0;
    for (int j = 0; j < kTicksToDouble; ++j) {
        d += std::exp(kGrowthBeta * static_cast<double>(j));
    }
    return d;
}

const double gGrowthDenom = growthDenom();

} // namespace

CrashGame::CrashGame(int startingBet)
    : currentBet_(startingBet)
{
}

int CrashGame::currentBet() const
{
    return currentBet_;
}

CrashPhase CrashGame::phase() const
{
    return phase_;
}

CrashOutcome CrashGame::lastOutcome() const
{
    return lastOutcome_;
}

double CrashGame::multiplier() const
{
    return multiplier_;
}

double CrashGame::crashPoint() const
{
    return crashPoint_;
}

int CrashGame::roundBet() const
{
    return roundBet_;
}

int CrashGame::lastPayout() const
{
    return lastPayout_;
}

void CrashGame::increaseBet(const Wallet& wallet)
{
    if (phase_ != CrashPhase::Idle) return;
    if (currentBet_ + 10 <= wallet.balance()) {
        currentBet_ += 10;
    }
}

void CrashGame::decreaseBet()
{
    if (phase_ != CrashPhase::Idle) return;
    if (currentBet_ > 10) {
        currentBet_ -= 10;
    }
}

bool CrashGame::startRound(Wallet& wallet, RNG& rng)
{
    if (phase_ != CrashPhase::Idle) return false;
    if (!wallet.canBet(currentBet_)) return false;

    wallet.debit(currentBet_);
    roundBet_ = currentBet_;
    multiplier_ = 1.0;
    tickCount_ = 0;
    lastOutcome_ = CrashOutcome::None;
    lastPayout_ = 0;

    double u = rng.uniform01();
    constexpr double eps = 1e-9;
    u = std::clamp(u, eps, 1.0 - eps);
    constexpr double rtp = 0.97;
    crashPoint_ = rtp / (1.0 - u);
    crashPoint_ = std::max(1.01, std::min(crashPoint_, 10000.0));

    phase_ = CrashPhase::Flying;
    return true;
}

void CrashGame::cashOut(Wallet& wallet)
{
    if (phase_ != CrashPhase::Flying) return;

    lastPayout_ = static_cast<int>(static_cast<double>(roundBet_) * multiplier_);
    if (lastPayout_ > 0) {
        wallet.credit(lastPayout_);
    }
    lastOutcome_ = CrashOutcome::CashedOut;
    phase_ = CrashPhase::RoundOver;
}

void CrashGame::tick()
{
    if (phase_ != CrashPhase::Flying) return;

    const int k = tickCount_;
    tickCount_ += 1;
    const double weight = std::exp(kGrowthBeta * static_cast<double>(k));
    multiplier_ *= std::pow(2.0, weight / gGrowthDenom);

    if (multiplier_ >= crashPoint_) {
        multiplier_ = crashPoint_;
        lastOutcome_ = CrashOutcome::Crashed;
        lastPayout_ = 0;
        phase_ = CrashPhase::RoundOver;
    }
}

void CrashGame::acknowledgeRound()
{
    if (phase_ == CrashPhase::RoundOver) {
        phase_ = CrashPhase::Idle;
        lastOutcome_ = CrashOutcome::None;
        multiplier_ = 1.0;
        tickCount_ = 0;
        roundBet_ = 0;
    }
}

void CrashGame::clampBetToWallet(const Wallet& wallet)
{
    if (wallet.balance() > 0 && currentBet_ > wallet.balance()) {
        currentBet_ = wallet.balance();
    }
    if (currentBet_ < 10 && wallet.balance() >= 10) {
        currentBet_ = 10;
    }
}
