#pragma once

#include "rng.hpp"
#include "wallet.hpp"

/**
 * @brief Stores the result of a slot machine spin.
 *
 * This struct contains all relevant information from a completed spin,
 * including the symbols on each reel, the resulting multiplier, the
 * payout amount, and whether the player won.
 *
 * @author Daniel
 */
struct SlotsResult 
{
    int reelA = 0;
    int reelB = 1;
    int reelC = 2;
    int multiplier = 0;
    int payout = 0;
    bool won = false;
};

/**
 * @brief Represents the slot machine game logic.
 *
 * This class manages the slot machine gameplay, including betting,
 * spinning reels, computing payouts, and interacting with the
 * player's wallet.
 *
 * @author Daniel
 */
class SlotsGame 
{
public:
    /**
     * @brief Constructs a SlotsGame object.
     *
     * This constructor initializes the slot machine with a starting
     * bet amount.
     *
     * @param startingBet The initial bet amount.
     *
     * @author Daniel
     */
    explicit SlotsGame(int startingBet = 50);

    /**
     * @brief Returns the current bet amount.
     *
     * This function provides the bet value that will be used for
     * the next spin.
     *
     * @return The current bet amount.
     *
     * @author Daniel
     */
    int currentBet() const;

    /**
     * @brief Increases the bet amount.
     *
     * This function raises the current bet while ensuring it does
     * not exceed the player's wallet balance.
     *
     * @param wallet The player's wallet used to validate the bet.
     *
     * @return None.
     *
     * @author Daniel
     */
    void increaseBet(const Wallet& wallet);

    /**
     * @brief Decreases the bet amount.
     *
     * This function lowers the current bet while ensuring it does
     * not drop below the minimum allowed value.
     *
     * @return None.
     *
     * @author Daniel
     */
    void decreaseBet();

    /**
     * @brief Performs a slot machine spin.
     *
     * This function deducts the bet, generates random reel values,
     * computes the multiplier and payout, and updates the player's
     * wallet accordingly.
     *
     * @param wallet The player's wallet used for betting and payouts.
     * @param rng The random number generator used for reel outcomes.
     *
     * @return A SlotsResult containing the outcome of the spin.
     *
     * @author Daniel
     */
    SlotsResult spin(Wallet& wallet, RNG& rng);

private:
    /**
     * @brief Generates a random symbol for a reel.
     *
     * This function selects a symbol using weighted probabilities.
     *
     * @param rng The random number generator used for selection.
     *
     * @return An integer representing the selected symbol.
     *
     * @author Daniel
     */
    static int spinSymbol(RNG& rng);

    /**
     * @brief Computes the payout multiplier for a spin.
     *
     * This function determines the multiplier based on the combination
     * of symbols across the three reels.
     *
     * @param a The symbol on the first reel.
     * @param b The symbol on the second reel.
     * @param c The symbol on the third reel.
     *
     * @return The multiplier applied to the bet.
     *
     * @author Daniel
     */
    static int computeMultiplier(int a, int b, int c);

    int currentBet_;
};