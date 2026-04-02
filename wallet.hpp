#pragma once

/**
 * @brief Represents a player's wallet for managing coins.
 *
 * This class handles all wallet-related operations including checking
 * balance, validating bets, deducting coins for bets, and adding winnings.
 * It ensures that all transactions are valid and prevents invalid operations.
 *
 * @author Raiyan
 */
class Wallet 
{
public:
    /**
     * @brief Constructs a Wallet with an initial coin balance.
     *
     * Initializes the wallet with the specified number of coins.
     *
     * @param startingCoins The initial amount of coins in the wallet.
     *
     * @author Raiyan
     */
    explicit Wallet(int startingCoins) : coins_(startingCoins) {}

    /**
     * @brief Returns the current wallet balance.
     *
     * @return The number of coins currently in the wallet.
     *
     * @author Raiyan
     */
    int balance() const
    {
        return coins_;
    }

    /**
     * @brief Checks if a bet amount is valid.
     *
     * A valid bet must be greater than zero and less than or equal
     * to the current wallet balance.
     *
     * @param amount The bet amount to validate.
     *
     * @return True if the bet is valid, false otherwise.
     *
     * @author Raiyan
     */
    bool canBet(int amount) const
    {
        return amount > 0 && amount <= coins_;
    }

    /**
     * @brief Deducts coins from the wallet.
     *
     * This function subtracts the specified amount from the wallet
     * if the bet is valid.
     *
     * @param amount The number of coins to deduct.
     *
     * @return True if the transaction was successful, false otherwise.
     *
     * @author Raiyan
     */
    bool debit(int amount)
    {
        if (!canBet(amount)) return false;
        coins_ -= amount;
        return true;
    }

    /**
     * @brief Adds coins to the wallet.
     *
     * This function increases the wallet balance by the specified amount,
     * typically used for winnings.
     *
     * @param amount The number of coins to add.
     *
     * @return None.
     *
     * @author Raiyan
     */
    void credit(int amount)
    {
        if (amount > 0) coins_ += amount;
    }

private:
    /// @brief The current number of coins in the wallet.
    int coins_;
};