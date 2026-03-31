#include "rng.hpp"

RNG::RNG(std::optional<unsigned int> seed)
{
    if (seed.has_value()) {
        eng_.seed(*seed);
    } else {
        std::random_device rd;
        eng_.seed(rd());
    }
}

int RNG::uniformInt(int lo, int hi)
{
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(eng_);
}

double RNG::uniform01()
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(eng_);
}
