#include "model/topography.h"
#include <iostream>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

// Topography generation settings
float constexpr VERTEX_SPACING = 20.0f;
int constexpr NOISE_OCTAVES = 4;
float constexpr NOISE_LACUNARITY = 2.0f;
float constexpr NOISE_GAIN = 0.5f;
float constexpr NOISE_RIDGE_OFFSET = 1.0f;

Topography::Topography(const unsigned char seed, const float scale, 
    const int type, const int size, const int amplitude) :
    seed(seed), size(size), amplitude(amplitude),
    scale(scale), spacing(VERTEX_SPACING), type(type)
{
    setMinMaxHeight();
}

Topography::~Topography() {}

void Topography::setMinMaxHeight() {
    float minY = FLT_MAX;
    float maxY = -FLT_MAX;
    for (double z = 0; z < size; z += spacing) {
        for (double x = 0; x < size; x += spacing) {
            float y = 0;
            if (type == 0) 
                y = stb_perlin_fbm_noise3_seed(
                    x * scale, z * scale, 
                    0.0f, NOISE_LACUNARITY, NOISE_GAIN, 
                    NOISE_OCTAVES, seed) * amplitude;

            else if (type == 1)
                y = stb_perlin_ridge_noise3_seed(
                    x * scale, z * scale,
                    0.0f, NOISE_LACUNARITY, NOISE_GAIN, 
                    NOISE_RIDGE_OFFSET, NOISE_OCTAVES, seed) * amplitude;

            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }
    }

    this->minY = minY;
    this->maxY = maxY;
}

int Topography::getY(const int x, const int z) const {
    int y = 0;

    if (type == 0)
        y = stb_perlin_fbm_noise3_seed(
            x * scale, z * scale, 
            0.0f, NOISE_LACUNARITY, NOISE_GAIN, 
            NOISE_OCTAVES, seed) * amplitude;

    else if (type == 1)
        y = stb_perlin_ridge_noise3_seed(
            x * scale, z * scale, 
            0.0f, NOISE_LACUNARITY, NOISE_GAIN, 
            NOISE_RIDGE_OFFSET, NOISE_OCTAVES, seed) * amplitude;
    else
        std::cerr << "ERROR: getY() on topography with invalid topography type" << std::endl;

    y = ((y - minY) * amplitude) / (maxY - minY); // Normalize height
    return y;
}

float Topography::getMinY() const {
    return minY;
}

float Topography::getMaxY() const {
    return maxY;
}

unsigned char Topography::getSeed() const {
    return seed;
}

void Topography::setSeed(const unsigned char seed) {
    this->seed = seed;
}

int Topography::getType() const {
    return type;
}

void Topography::setType(const int type) {
    this->type = type;
}

float Topography::getScale() const {
    return scale;
}

int Topography::getSize() const {
    return size;
}

int Topography::getAmplitude() const {
    return amplitude;
}

void Topography::setScale(const float scale) {
    this->scale = scale;
}

float Topography::getSpacing() const {
    return spacing;
}
