#pragma once

#include <optional>
#include <random>

class RNG {
public:
    explicit RNG(std::optional<unsigned int> seed = std::nullopt);

    int uniformInt(int lo, int hi);

    double uniform01();

private:
    std::mt19937 eng_;
};
