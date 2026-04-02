#pragma once

#include <optional>
#include <random>

/**
 * @brief Random number generator utility class.
 *
 * This class provides an abstraction over the C++ Mersenne Twister engine
 * to generate random integers and floating-point values. It supports both
 * deterministic behavior through seeding and non-deterministic behavior
 * using a random device.
 *
 * @author Raiyan
 */
class RNG 
{
public:
    /**
     * @brief Constructs the random number generator.
     *
     * Initializes the internal engine using either a user-provided seed
     * or a randomly generated seed if none is provided.
     *
     * @param seed Optional seed value for deterministic random generation.
     *
     * @author Raiyan
     */
    explicit RNG(std::optional<unsigned int> seed = std::nullopt);

    /**
     * @brief Generates a random integer within a specified range.
     *
     * This function produces a uniformly distributed integer between
     * the given lower and upper bounds (inclusive).
     *
     * @param lo The lower bound of the range.
     * @param hi The upper bound of the range.
     *
     * @return A random integer between lo and hi (inclusive).
     *
     * @author Raiyan
     */
    int uniformInt(int lo, int hi);

    /**
     * @brief Generates a random floating-point value between 0 and 1.
     *
     * This function produces a uniformly distributed double in the
     * range [0.0, 1.0], commonly used for probability calculations.
     *
     * @return A random double between 0.0 and 1.0.
     *
     * @author Raiyan
     */
    double uniform01();

private:
    /// @brief Internal Mersenne Twister random engine.
    std::mt19937 eng_;
};