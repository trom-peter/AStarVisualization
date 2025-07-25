#pragma once

#include <random>

struct SeedGenerator {
    static int getRandomSeed() {
        static std::random_device rd;
        static std::mt19937 mt{ rd() };
        std::uniform_int_distribution<> dist{ std::numeric_limits<int>::min(), std::numeric_limits<int>::max() };
        return dist(mt);
    }
};
