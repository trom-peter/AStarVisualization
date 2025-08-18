#pragma once
#include <random>

struct SeedGenerator {
    // Return random seed in range (0, 255)
    static unsigned char getRandomSeed() {
        static std::random_device rd;
        static std::mt19937 mt{ rd() };
        std::uniform_int_distribution<> dist{ std::numeric_limits<unsigned char>::min(), 
            std::numeric_limits<unsigned char>::max() };

        return dist(mt);
    }
};
