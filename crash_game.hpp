#pragma once

#include "rng.hpp"
#include "wallet.hpp"

/**
 * @brief Represents the current phase of the crash game.
 *
 * This enum class is used to track the state of the crash game,
 * indicating whether the game is idle, actively running (flying),
 * or has completed a round.
 *
 * @author Daniel
 */
enum class CrashPhase 
{
    Idle,
    Flying,
    RoundOver
};

/**
 * @brief Represents the outcome of a crash round.
 *
 * This enum class stores the result of the most recent round,
 * indicating whether the player cashed out successfully, the
 * round crashed, or no outcome has been determined yet.
 *
 * @author Daniel
 */
enum class CrashOutcome 
{
    None,
    CashedOut,
    Crashed
};

/**
 * @brief Represents the crash game logic and state.
 *
 * This class manages all aspects of the crash game, including
 * betting, multiplier progression, crash point generation,
 * round outcomes, and interactions with the player's wallet.
 *
 * @author Daniel
 */
class CrashGame 
{
public:
    /**
     * @brief Constructs a CrashGame object.
     *
     * This constructor initializes the crash game with a starting
     * bet amount used before the first round begins.
     *
     * @param startingBet The initial bet amount.
     *
     * @author Daniel
     */
    explicit CrashGame(int startingBet = 50);

    /**
     * @brief Returns the currently selected bet amount.
     *
     * This function provides the bet value chosen by the player
     * for the next round.
     *
     * @return The current bet amount.
     *
     * @author Daniel
     */
    int currentBet() const;

    /**
     * @brief Returns the current phase of the game.
     *
     * This function indicates whether the game is idle, actively
     * running, or has completed a round.
     *
     * @return The current crash phase.
     *
     * @author Daniel
     */
    CrashPhase phase() const;

    /**
     * @brief Returns the outcome of the last round.
     *
     * This function provides the result of the most recently
     * completed round.
     *
     * @return The last crash outcome.
     *
     * @author Daniel
     */
    CrashOutcome lastOutcome() const;

    /**
     * @brief Returns the current multiplier.
     *
     * This function provides the multiplier value that increases
     * during an active round.
     *
     * @return The current multiplier value.
     *
     * @author Daniel
     */
    double multiplier() const;

    /**
     * @brief Returns the crash point for the round.
     *
     * This function provides the multiplier at which the round
     * will crash automatically.
     *
     * @return The crash point multiplier.
     *
     * @author Daniel
     */
    double crashPoint() const;

    /**
     * @brief Returns the bet placed in the current round.
     *
     * This function provides the wager that was deducted from
     * the wallet at the start of the round.
     *
     * @return The round bet amount.
     *
     * @author Daniel
     */
    int roundBet() const;

    /**
     * @brief Returns the payout from the last round.
     *
     * This function provides the amount of coins returned to
     * the player after the most recent round.
     *
     * @return The last payout amount.
     *
     * @author Daniel
     */
    int lastPayout() const;

    /**
     * @brief Increases the selected bet amount.
     *
     * This function increases the bet by a fixed increment
     * if the game is idle and the wallet allows it.
     *
     * @param wallet The player's wallet used to validate the bet.
     *
     * @return None.
     *
     * @author Daniel
     */
    void increaseBet(const Wallet& wallet);

    /**
     * @brief Decreases the selected bet amount.
     *
     * This function reduces the bet by a fixed amount if the
     * game is idle and the bet is above the minimum value.
     *
     * @return None.
     *
     * @author Daniel
     */
    void decreaseBet();

    /**
     * @brief Starts a new crash round.
     *
     * This function validates the bet, debits the wallet,
     * initializes round variables, and generates a crash point.
     *
     * @param wallet The player's wallet used to place the bet.
     * @param rng The random number generator used for crash calculation.
     *
     * @return True if the round started successfully, otherwise false.
     *
     * @author Daniel
     */
    bool startRound(Wallet& wallet, RNG& rng);

    /**
     * @brief Cashes out the current round.
     *
     * This function allows the player to collect winnings based on
     * the current multiplier before a crash occurs.
     *
     * @param wallet The player's wallet used to receive the payout.
     *
     * @return None.
     *
     * @author Daniel
     */
    void cashOut(Wallet& wallet);

    /**
     * @brief Advances the round by one tick.
     *
     * This function updates the multiplier progression and checks
     * if the crash point has been reached.
     *
     * @return None.
     *
     * @author Daniel
     */
    void tick();

    /**
     * @brief Resets the game after a completed round.
     *
     * This function clears round-specific data and returns the
     * game to the idle state.
     *
     * @return None.
     *
     * @author Daniel
     */
    void acknowledgeRound();

    /**
     * @brief Ensures the bet is valid relative to the wallet.
     *
     * This function adjusts the current bet so that it does not
     * exceed the wallet balance and respects minimum constraints.
     *
     * @param wallet The player's wallet used for validation.
     *
     * @return None.
     *
     * @author Daniel
     */
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