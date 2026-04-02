#include "crash_game.hpp"

#include <algorithm>
#include <cmath>

namespace 
{

constexpr int kTicksToDouble = 11;
constexpr double kGrowthBeta = 0.11;

/**
 * @brief Computes the normalization denominator for crash multiplier growth.
 *
 * This helper function calculates the sum of exponential growth weights used
 * to control how the crash multiplier increases over time. The resulting value
 * is used to normalize growth so that the multiplier progression follows the
 * intended curve.
 *
 * @return The computed growth denominator used for multiplier scaling.
 *
 * @author Daniel
 */
double growthDenom()
{
    double d = 0.0;
    for (int j = 0; j < kTicksToDouble; ++j) 
    {
        d += std::exp(kGrowthBeta * static_cast<double>(j));
    }
    return d;
}

const double gGrowthDenom = growthDenom();

} // namespace

/**
 * @brief Constructs a CrashGame object with an initial bet amount.
 *
 * This constructor initializes the crash game and sets the starting bet
 * amount that will be used before the first round begins.
 *
 * @param startingBet The initial bet amount for the game.
 *
 * @author Daniel
 */
CrashGame::CrashGame(int startingBet)
    : currentBet_(startingBet)
{
}

/**
 * @brief Returns the currently selected bet amount.
 *
 * This function provides the current wager chosen by the player for the
 * next crash round.
 *
 * @return The current selected bet amount.
 *
 * @author Daniel
 */
int CrashGame::currentBet() const
{
    return currentBet_;
}

/**
 * @brief Returns the current phase of the crash game.
 *
 * This function indicates whether the game is idle, actively running,
 * or has completed a round.
 *
 * @return The current crash game phase.
 *
 * @author Daniel
 */
CrashPhase CrashGame::phase() const
{
    return phase_;
}

/**
 * @brief Returns the outcome of the most recent round.
 *
 * This function reports whether the player cashed out successfully,
 * crashed, or if no round result is currently stored.
 *
 * @return The outcome of the most recently completed round.
 *
 * @author Daniel
 */
CrashOutcome CrashGame::lastOutcome() const
{
    return lastOutcome_;
}

/**
 * @brief Returns the current multiplier value.
 *
 * This function provides the live multiplier during an active round,
 * or the last multiplier value reached before the round ended.
 *
 * @return The current crash multiplier.
 *
 * @author Daniel
 */
double CrashGame::multiplier() const
{
    return multiplier_;
}

/**
 * @brief Returns the crash point for the current round.
 *
 * This function provides the multiplier value at which the round
 * will automatically crash.
 *
 * @return The crash point multiplier for the round.
 *
 * @author Daniel
 */
double CrashGame::crashPoint() const
{
    return crashPoint_;
}

/**
 * @brief Returns the bet amount placed in the current or last round.
 *
 * This function provides the wager that was debited when the current
 * round started.
 *
 * @return The bet amount for the round.
 *
 * @author Daniel
 */
int CrashGame::roundBet() const
{
    return roundBet_;
}

/**
 * @brief Returns the payout from the most recent round.
 *
 * This function reports the amount credited back to the player's wallet
 * after the previous round. It is zero if the round ended in a crash.
 *
 * @return The payout from the last completed round.
 *
 * @author Daniel
 */
int CrashGame::lastPayout() const
{
    return lastPayout_;
}

/**
 * @brief Increases the selected bet amount.
 *
 * This function raises the current bet by 10 only if the game is idle
 * and the new value does not exceed the wallet balance.
 *
 * @param wallet The player's wallet used to validate the bet increase.
 *
 * @return None.
 *
 * @author Daniel
 */
void CrashGame::increaseBet(const Wallet& wallet)
{
    if (phase_ != CrashPhase::Idle) return;

    if (currentBet_ + 10 <= wallet.balance()) 
    {
        currentBet_ += 10;
    }
}

/**
 * @brief Decreases the selected bet amount.
 *
 * This function lowers the current bet by 10 only if the game is idle
 * and the current bet is above the minimum threshold.
 *
 * @return None.
 *
 * @author Daniel
 */
void CrashGame::decreaseBet()
{
    if (phase_ != CrashPhase::Idle) return;

    if (currentBet_ > 10) 
    {
        currentBet_ -= 10;
    }
}

/**
 * @brief Starts a new crash round.
 *
 * This function validates the current bet, debits the wallet, resets round
 * state, generates a new crash point, and changes the game phase to flying.
 *
 * @param wallet The player's wallet used to place the bet.
 * @param rng The random number generator used to generate the crash point.
 *
 * @return True if the round started successfully, otherwise false.
 *
 * @author Daniel
 */
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

/**
 * @brief Cashes out the current round.
 *
 * This function allows the player to collect winnings based on the current
 * multiplier if the rocket has not yet crashed. It updates the payout,
 * credits the wallet, and ends the round.
 *
 * @param wallet The player's wallet used to receive the payout.
 *
 * @return None.
 *
 * @author Daniel
 */
void CrashGame::cashOut(Wallet& wallet)
{
    if (phase_ != CrashPhase::Flying) return;

    lastPayout_ = static_cast<int>(static_cast<double>(roundBet_) * multiplier_);
    if (lastPayout_ > 0) 
    {
        wallet.credit(lastPayout_);
    }
    lastOutcome_ = CrashOutcome::CashedOut;
    phase_ = CrashPhase::RoundOver;
}

/**
 * @brief Advances the crash round by one tick.
 *
 * This function updates the multiplier according to the configured growth
 * curve and checks whether the multiplier has reached or exceeded the crash
 * point. If so, the round ends as a crash.
 *
 * @return None.
 *
 * @author Daniel
 */
void CrashGame::tick()
{
    if (phase_ != CrashPhase::Flying) return;

    const int k = tickCount_;
    tickCount_ += 1;
    const double weight = std::exp(kGrowthBeta * static_cast<double>(k));
    multiplier_ *= std::pow(2.0, weight / gGrowthDenom);

    if (multiplier_ >= crashPoint_) 
    {
        multiplier_ = crashPoint_;
        lastOutcome_ = CrashOutcome::Crashed;
        lastPayout_ = 0;
        phase_ = CrashPhase::RoundOver;
    }
}

/**
 * @brief Acknowledges a completed round and resets round state.
 *
 * This function clears the stored outcome and round-specific values after
 * a round has ended, returning the game to the idle state so that a new
 * round can begin.
 *
 * @return None.
 *
 * @author Daniel
 */
void CrashGame::acknowledgeRound()
{
    if (phase_ == CrashPhase::RoundOver) 
    {
        phase_ = CrashPhase::Idle;
        lastOutcome_ = CrashOutcome::None;
        multiplier_ = 1.0;
        tickCount_ = 0;
        roundBet_ = 0;
    }
}

/**
 * @brief Adjusts the current bet to fit within wallet limits.
 *
 * This function ensures that the selected bet does not exceed the player's
 * balance and enforces the minimum bet when the wallet still contains at
 * least that amount.
 *
 * @param wallet The player's wallet used to validate the selected bet.
 *
 * @return None.
 *
 * @author Daniel
 */
void CrashGame::clampBetToWallet(const Wallet& wallet)
{
    if (wallet.balance() > 0 && currentBet_ > wallet.balance()) {
        currentBet_ = wallet.balance();
    }
    if (currentBet_ < 10 && wallet.balance() >= 10) 
    {
        currentBet_ = 10;
    }
}