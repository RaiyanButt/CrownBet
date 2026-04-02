#include "rng.hpp"

/**
 * @brief Constructs the random number generator.
 *
 * This constructor initializes the internal random engine. If a seed is
 * provided, the generator will produce a deterministic sequence of values.
 * Otherwise, it uses std::random_device to generate a random seed for
 * non-deterministic behavior.
 *
 * @param seed Optional seed value used to initialize the random engine.
 *
 * @author Raiyan
 */
RNG::RNG(std::optional<unsigned int> seed)
{
    if (seed.has_value()) 
    {
        eng_.seed(*seed);
    } 
    else 
    {
        std::random_device rd;
        eng_.seed(rd());
    }
}

/**
 * @brief Generates a uniformly distributed random integer.
 *
 * This function returns a random integer within the inclusive range
 * [lo, hi] using the internal random engine.
 *
 * @param lo The lower bound of the range.
 * @param hi The upper bound of the range.
 *
 * @return A random integer between lo and hi (inclusive).
 *
 * @author Raiyan
 */
int RNG::uniformInt(int lo, int hi)
{
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(eng_);
}

/**
 * @brief Generates a uniformly distributed random double between 0 and 1.
 *
 * This function returns a random floating-point value in the range
 * [0.0, 1.0], which is useful for probability-based calculations.
 *
 * @return A random double between 0.0 and 1.0.
 *
 * @author Raiyan
 */
double RNG::uniform01()
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(eng_);
}