#include "blackjack_game.hpp"

/**
 * @brief Constructs a BlackjackGame object with an initial bet amount.
 *
 * @param startingBet The initial bet amount for the game.
 *
 * @author Alexander
 */
BlackjackGame::BlackjackGame(int startingBet)
    : currentBet_(startingBet)
{
}

/**
 * @brief Returns the current selected bet amount.
 * 
 * @return The current selected bet amount 
 *
 * @author Alexander
*/
int BlackjackGame::currentBet() const
{
    return currentBet_;
}

/**
 * @brief Returns the current phase of the blackjack game.
 *
 * @return The current blackjack phase.
 *
 * @author Alexander
 */
BlackjackPhase BlackjackGame::phase() const
{
    return phase_;
}

/**
 * @brief Returns the outcome of the most recent round.
 *
 * @return The outcome of the round.
 *
 * @author Alexander
 */
BlackjackOutcome BlackjackGame::outcome() const
{
    return outcome_;
}

/**
 * @brief Returns the bet amount used in the current or most recent round.
 *
 * @return The bet amount for the round.
 *
 * @author Alexander
 */
int BlackjackGame::roundBet() const
{
    return roundBet_;
}

/**
 * @brief Returns the payout from the most recently completed round.
 *
 * @return The most recent payout amount.
 *
 * @author Alexander
 */
int BlackjackGame::lastPayout() const
{
    return lastPayout_;
}

/**
 * @brief Indicates whether the most recent winning round was a natural blackjack.
 *
 * @return True if the last win was by blackjack, otherwise false.
 *
 * @author Alexander 
 */
bool BlackjackGame::lastWinWasBlackjack() const
{
    return lastWinByBlackjack_;
}

/**
 * @brief Returns the player's current hand.
 *
 * @return A constant reference to the player's hand.
 *
 * @author Alexander 
 */
const std::vector<BlackjackCard>& BlackjackGame::playerCards() const
{
    return player_;
}

/**
 * @brief Returns the dealer's current hand.
 *
 * @return A constant reference to the dealer's hand.
 *
 * @author Alexander 
 */
const std::vector<BlackjackCard>& BlackjackGame::dealerCards() const
{
    return dealer_;
}

/**
 * @brief Determines whether the dealer's hole card should remain hidden.
 *
 * @return True if the dealer's hole card should be hidden, otherwise false.
 *
 * @author Alexander
 */
bool BlackjackGame::dealerHoleHidden() const
{
    return phase_ == BlackjackPhase::PlayerTurn && dealer_.size() >= 2;
}

/**
 * @brief Calculates and returns the total value of the player's hand.
 *
 * @return The total value of the player's hand.
 *
 * @author Alexander 
 */
int BlackjackGame::playerTotal() const
{
    return handTotal(player_);
}

/**
 * @brief Calculates and returns the total value of the dealer's hand.
 *
 * @return The total value of the dealer's hand.
 *
 * @author Alexander 
 */
int BlackjackGame::dealerTotal() const
{
    return handTotal(dealer_);
}

/**
 * @brief Increases the current bet if allowed.
 *
 * @param wallet The player's wallet used to check the available balance.
 *
 * @author Alexander
 */
void BlackjackGame::increaseBet(const Wallet& wallet)
{
    if (phase_ != BlackjackPhase::Idle) return;

    if (currentBet_ < wallet.balance()) 
    {
        currentBet_ += 10;
    }
}

/**
 * @brief Decreases the current bet if allowed.
 *
 * @author Alexander 
 */
void BlackjackGame::decreaseBet()
{
    if (phase_ != BlackjackPhase::Idle) return;
    if (currentBet_ > 10) {
        currentBet_ -= 10;
    }
}

/**
 * @brief Adjusts the current bet so it stays within valid wallet limits.
 *
 * This function ensures that the selected bet does not exceed the wallet
 * balance and respects the minimum bet value whenever possible.
 *
 * @param wallet The player's wallet used to validate the bet amount.
 *
 * @author Alexander 
 */
void BlackjackGame::clampBetToWallet(const Wallet& wallet)
{
    if (currentBet_ > wallet.balance()) 
    {
        currentBet_ = wallet.balance();
    }

    if (currentBet_ < 10) 
    {
        currentBet_ = 10;
    }

    if (wallet.balance() < 10) 
    {
        currentBet_ = wallet.balance();
    }
}

/**
 * @brief Computes the total value of a blackjack hand.
 *
 * This function sums the values of all cards in the given hand while treating
 * aces as 11 initially and reducing them to 1 as needed to avoid busting.
 *
 * @param hand The hand of cards to evaluate.
 *
 * @return The total blackjack value of the hand.
 *
 * @author Alexander
 */
int BlackjackGame::handTotal(const std::vector<BlackjackCard>& hand)
{
    int sum = 0;
    int aces = 0;

    for (const BlackjackCard& c : hand) 
    {
        if (c.isAce) 
        {
            ++aces;
        } 
        else 
        {
            sum += c.value;
        }
    }

    sum += aces * 11;

    while (sum > 21 && aces > 0) 
    {
        sum -= 10;
        --aces;
    }
    return sum;
}

/**
 * @brief Determines whether a hand is a natural blackjack.
 *
 * @param hand The hand of cards to evaluate.
 *
 * @return True if the hand is a natural blackjack, otherwise false.
 *
 * @author Alexander
 */
bool BlackjackGame::isNatural(const std::vector<BlackjackCard>& hand)
{
    return hand.size() == 2 && handTotal(hand) == 21;
}

/**
 * @brief Draws a random blackjack card.
 *
 * This function generates a random card rank and converts it into a
 * BlackjackCard object. Face cards count as 10, number cards retain
 * their rank, and aces are marked separately.
 *
 * @param rng The random number generator used to draw the card.
 *
 * @return A randomly generated blackjack card.
 *
 * @author Alexander
 */
BlackjackCard BlackjackGame::drawCard(RNG& rng)
{
    const int r = rng.uniformInt(1, 13);

    if (r == 1) 
    {
        return BlackjackCard{true, 0};
    }

    if (r >= 10) 
    {
        return BlackjackCard{false, 10};
    }

    return BlackjackCard{false, r};
}

/**
 * @brief Starts a new blackjack round by dealing cards to the player and dealer.
 *
 * This function checks that the game is idle and that the wallet contains
 * enough funds for the selected bet. It debits the bet, resets round state,
 * deals two cards to both the player and dealer, and automatically stands
 * if the player is dealt 21.
 *
 * @param wallet The player's wallet used to place the bet and receive payouts.
 * @param rng The random number generator used to deal cards.
 *
 * @return True if the hand was successfully dealt, otherwise false.
 *
 * @author Alexander 
 */
bool BlackjackGame::deal(Wallet& wallet, RNG& rng)
{
    if (phase_ != BlackjackPhase::Idle) 
    {
        return false;
    }

    if (!wallet.canBet(currentBet_)) 
    {
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

    if (handTotal(player_) == 21) 
    {
        stand(wallet, rng);
    }

    return true;
}

/**
 * @brief Draws one additional card for the player.
 *
 * This function is only available during the player's turn. After drawing,
 * it checks whether the player busts or reaches 21, automatically ending
 * the round or standing when appropriate.
 *
 * @param wallet The player's wallet used during round resolution if needed.
 * @param rng The random number generator used to draw the card.
 *
 * @author Alexander
 */
void BlackjackGame::hit(Wallet& wallet, RNG& rng)
{
    if (phase_ != BlackjackPhase::PlayerTurn) 
    {
        return;
    }

    player_.push_back(drawCard(rng));
    const int t = handTotal(player_);

    if (t > 21) 
    {
        lastPayout_ = 0;
        lastWinByBlackjack_ = false;
        outcome_ = BlackjackOutcome::Lose;
        phase_ = BlackjackPhase::RoundOver;
    } 
    else if (t == 21) 
    {
        stand(wallet, rng);
    }
}

/**
 * @brief Resolves the round by comparing the player and dealer hands.
 *
 * This function determines the final outcome after the dealer finishes drawing.
 * It handles natural blackjacks, busts, standard wins and losses, and pushes,
 * while also updating the player's wallet and payout information.
 *
 * @param wallet The player's wallet used to credit any winnings or returned bets.
 *
 * @author Alexander
 */
void BlackjackGame::resolveAgainstDealer(Wallet& wallet)
{
    const int pt = handTotal(player_);
    const int dt = handTotal(dealer_);
    const bool playerNatural = isNatural(player_);
    const bool dealerNatural = isNatural(dealer_);

    lastPayout_ = 0;
    lastWinByBlackjack_ = false;

    if (pt > 21) 
    {
        outcome_ = BlackjackOutcome::Lose;
        return;
    }

    if (playerNatural && dealerNatural) 
    {
        outcome_ = BlackjackOutcome::Push;
        wallet.credit(roundBet_);
        lastPayout_ = roundBet_;
        return;
    }

    if (playerNatural) 
    {
        outcome_ = BlackjackOutcome::Win;
        wallet.credit(roundBet_ * 3);
        lastPayout_ = roundBet_ * 3;
        lastWinByBlackjack_ = true;
        return;
    }

    if (dealerNatural) 
    {
        outcome_ = BlackjackOutcome::Lose;
        return;
    }

    if (dt > 21) 
    {
        outcome_ = BlackjackOutcome::Win;
        wallet.credit(roundBet_ * 2);
        lastPayout_ = roundBet_ * 2;
        return;
    }

    if (dt > pt) 
    {
        outcome_ = BlackjackOutcome::Lose;
        return;
    }
    if (pt > dt) 
    {
        outcome_ = BlackjackOutcome::Win;
        wallet.credit(roundBet_ * 2);
        lastPayout_ = roundBet_ * 2;
        return;
    }

    outcome_ = BlackjackOutcome::Push;
    wallet.credit(roundBet_);
    lastPayout_ = roundBet_;
}

/**
 * @brief Ends the player's turn and plays out the dealer's hand.
 *
 * This function causes the dealer to draw until reaching a total of at least 17.
 * Once the dealer finishes, the round is resolved and marked as complete.
 *
 * @param wallet The player's wallet used when resolving the round outcome.
 * @param rng The random number generator used for drawing dealer cards.
 *
 * @author Alexander
 */
void BlackjackGame::stand(Wallet& wallet, RNG& rng)
{
    if (phase_ != BlackjackPhase::PlayerTurn) 
    {
        return;
    }

    while (handTotal(dealer_) < 17) 
    {
        dealer_.push_back(drawCard(rng));
    }

    resolveAgainstDealer(wallet);
    phase_ = BlackjackPhase::RoundOver;
}

/**
 * @brief Resets the game state to prepare for a new hand.
 *
 * This function clears both hands, resets the round outcome and payout data,
 * and returns the game to the idle phase so a new round can be started.
 *
 * @author Alexander Chronopoulos
 */
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
