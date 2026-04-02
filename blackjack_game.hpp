#pragma once

#include <vector>

#include "rng.hpp"
#include "wallet.hpp"

/**
 * @brief Represents the current phase of a blackjack round.
 *
 * This enum class is used to track the current state of gameplay.
 * It indicates whether the game is waiting to begin, whether the
 * player is actively taking actions, or whether the round has ended.
 *
 * @author Alexander
 */
enum class BlackjackPhase 
{
    Idle,
    PlayerTurn,
    RoundOver
};

/**
 * @brief Represents the result of a blackjack round.
 *
 * This enum class stores the final outcome of a completed hand.
 * It is used to indicate whether the player won, lost, pushed,
 * or if no result has been determined yet.
 *
 * @author Alexander 
 */
enum class BlackjackOutcome 
{
    None,
    Win,
    Lose,
    Push
};

/**
 * @brief Represents a single blackjack card.
 *
 * This structure stores the information needed to evaluate a card
 * in blackjack. A card may be marked as an ace, or it may store a
 * fixed numeric value from 2 to 10, where face cards are treated as 10.
 *
 * @author Alexander 
 */
struct BlackjackCard 
{
    /**
     * @brief Indicates whether the card is an ace.
     *
     * This value is true when the card is an ace and false otherwise.
     * Aces are handled separately because their value may count as
     * either 1 or 11 during hand evaluation.
     *
     * @author Alexander 
     */
    bool isAce = false;

    /**
     * @brief Stores the numeric value of the card when it is not an ace.
     *
     * This value ranges from 2 to 10. A value of 10 is also used for
     * face cards such as jack, queen, and king. This field is ignored
     * when the card is marked as an ace.
     *
     * @author Alexander 
     */
    int value = 0;
};

/**
 * @brief Represents a blackjack game session.
 *
 * This class manages the complete state and behavior of a blackjack game.
 * It stores the player's and dealer's hands, tracks betting information,
 * controls the round phase, evaluates totals and outcomes, and interacts
 * with the wallet and random number generator during play.
 *
 * @author Alexander
 */
class BlackjackGame 
{
public:
    /**
     * @brief Constructs a BlackjackGame object with an initial bet amount.
     *
     * This constructor initializes the blackjack game and sets the
     * starting bet value that will be used before the first hand begins.
     *
     * @param startingBet The initial bet amount for the game.
     *
     * @author Alexander 
     */
    explicit BlackjackGame(int startingBet = 50);

    /**
     * @brief Returns the current selected bet amount.
     *
     * This function provides the value of the bet that is currently
     * selected for the next round.
     *
     * @return The current selected bet amount.
     *
     * @author Alexander
     */
    int currentBet() const;

    /**
     * @brief Returns the current phase of the game.
     *
     * This function indicates whether the game is idle, in the player's
     * turn, or has finished the current round.
     *
     * @return The current blackjack phase.
     *
     * @author Alexander
     */
    BlackjackPhase phase() const;

    /**
     * @brief Returns the result of the most recent round.
     *
     * This function reports the round outcome, such as win, loss,
     * push, or no result yet.
     *
     * @return The outcome of the round.
     *
     * @author Alexander
     */
    BlackjackOutcome outcome() const;

    /**
     * @brief Returns the amount wagered in the current or last round.
     *
     * This function provides the bet that was committed when the hand
     * was dealt.
     *
     * @return The bet amount for the round.
     *
     * @author Alexander
     */
    int roundBet() const;

    /**
     * @brief Returns the payout from the most recently resolved round.
     *
     * This function reports how many coins were returned to the wallet
     * after the previous hand was resolved. It is zero when the player loses.
     *
     * @return The most recent payout amount.
     *
     * @author Alexander
     */
    int lastPayout() const;

    /**
     * @brief Indicates whether the last win was a natural blackjack.
     *
     * This function returns true if the player's most recent winning hand
     * was specifically a natural blackjack.
     *
     * @return True if the last win was by blackjack, otherwise false.
     *
     * @author Alexander
     */
    bool lastWinWasBlackjack() const;

    /**
     * @brief Returns the player's current hand.
     *
     * This function provides read-only access to the player's cards.
     *
     * @return A constant reference to the player's hand.
     *
     * @author Alexander
     */
    const std::vector<BlackjackCard>& playerCards() const;

    /**
     * @brief Returns the dealer's current hand.
     *
     * This function provides read-only access to the dealer's cards.
     *
     * @return A constant reference to the dealer's hand.
     *
     * @author Alexander 
     */
    const std::vector<BlackjackCard>& dealerCards() const;

    /**
     * @brief Determines whether the dealer's hidden card should remain concealed.
     *
     * This function is used during the player's turn to check whether the
     * dealer's second card should still be hidden from view.
     *
     * @return True if the dealer's hole card should be hidden, otherwise false.
     *
     * @author Alexander
     */
    bool dealerHoleHidden() const;

    /**
     * @brief Returns the total value of the player's hand.
     *
     * This function calculates the player's hand total using blackjack rules,
     * including proper ace handling.
     *
     * @return The total value of the player's hand.
     *
     * @author Alexander 
     */
    int playerTotal() const;

    /**
     * @brief Returns the total value of the dealer's hand.
     *
     * This function calculates the dealer's hand total using blackjack rules,
     * including proper ace handling.
     *
     * @return The total value of the dealer's hand.
     *
     * @author Alexander 
     */
    int dealerTotal() const;

    /**
     * @brief Increases the selected bet amount.
     *
     * This function raises the current bet while the game is idle,
     * as long as the wallet balance allows the increase.
     *
     * @param wallet The player's wallet used to validate the bet increase.
     *
     * @author Alexander 
     */
    void increaseBet(const Wallet& wallet);

    /**
     * @brief Decreases the selected bet amount.
     *
     * This function lowers the current bet while the game is idle,
     * ensuring that the bet does not go below the minimum allowed value.
     *
     * @author Alexander 
     */
    void decreaseBet();

    /**
     * @brief Adjusts the current bet to fit within wallet constraints.
     *
     * This function ensures that the selected bet does not exceed the
     * wallet balance and remains within valid limits.
     *
     * @param wallet The player's wallet used to validate the bet amount.
     *
     * @author Alexander 
     */
    void clampBetToWallet(const Wallet& wallet);

    /**
     * @brief Starts a new round by dealing cards to the player and dealer.
     *
     * This function places the current bet, resets round data, and deals
     * two cards to each side if the game is ready and the wallet contains
     * sufficient funds.
     *
     * @param wallet The player's wallet used to place the bet and receive payouts.
     * @param rng The random number generator used to draw cards.
     *
     * @return True if the round was successfully started, otherwise false.
     *
     * @author Alexander 
     */
    bool deal(Wallet& wallet, RNG& rng);

    /**
     * @brief Draws an additional card for the player.
     *
     * This function is used during the player's turn. After drawing,
     * it updates the hand state and checks for busting or reaching 21.
     *
     * @param wallet The player's wallet used if the round must be resolved.
     * @param rng The random number generator used to draw the card.
     *
     * @author Alexander 
     */
    void hit(Wallet& wallet, RNG& rng);

    /**
     * @brief Ends the player's turn and resolves the dealer's play.
     *
     * This function causes the dealer to draw cards until the dealer
     * reaches the required total, then resolves the outcome of the round.
     *
     * @param wallet The player's wallet used during round resolution.
     * @param rng The random number generator used to draw dealer cards.
     *
     * @author Alexander 
     */
    void stand(Wallet& wallet, RNG& rng);

    /**
     * @brief Resets the game state to prepare for a new hand.
     *
     * This function clears both hands, resets the round outcome and payout
     * information, and returns the game to the idle state.
     *
     * @author Alexander 
     */
    void newHand();

private:
    /**
     * @brief Calculates the total value of a blackjack hand.
     *
     * This helper function evaluates the cards in a hand according to
     * blackjack scoring rules, including adjusting aces from 11 to 1
     * when necessary to avoid busting.
     *
     * @param hand The hand of cards to evaluate.
     *
     * @return The total blackjack value of the hand.
     *
     * @author Alexander 
     */
    static int handTotal(const std::vector<BlackjackCard>& hand);

    /**
     * @brief Determines whether a hand is a natural blackjack.
     *
     * This helper function checks whether a hand contains exactly two cards
     * and totals 21.
     *
     * @param hand The hand of cards to evaluate.
     *
     * @return True if the hand is a natural blackjack, otherwise false.
     *
     * @author Alexander 
     */
    static bool isNatural(const std::vector<BlackjackCard>& hand);

    /**
     * @brief Draws a random blackjack card.
     *
     * This helper function generates a single random card for use in the game.
     * It maps random ranks to blackjack values, including ace handling and
     * face cards counting as 10.
     *
     * @param rng The random number generator used to draw the card.
     *
     * @return A randomly generated blackjack card.
     *
     * @author Alexander 
     */
    static BlackjackCard drawCard(RNG& rng);

    /**
     * @brief Compares the player and dealer hands to determine the round result.
     *
     * This helper function evaluates both hands after the dealer has finished
     * drawing and updates the round outcome, payout, and wallet balance.
     *
     * @param wallet The player's wallet used to credit any winnings or returned bets.
     *
     * @author Alexander 
     */
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