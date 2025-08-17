#include "model/topography.h"
#include <iostream>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

float constexpr VERTEX_SPACING = 20.0f;
int constexpr TOPOGRAPHY_AMPLITUDE = 500;
int constexpr NOISE_OCTAVES = 4;
float constexpr NOISE_LACUNARITY = 2.0f;
float constexpr NOISE_GAIN = 0.5f;
float constexpr NOISE_RIDGE_OFFSET = 1.0f;

Topography::Topography(unsigned char seed, float scale, int type, int size, int amplitude) :
    seed(seed), size(size), amplitude(TOPOGRAPHY_AMPLITUDE), 
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
            float y;
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

int Topography::getY(int x, int z) {
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

    y = ((y - minY) * amplitude) / (maxY - minY); //normalize
    return y;
}

float Topography::getMinY() {
    return minY;
}

float Topography::getMaxY() {
    return maxY;
}

unsigned char Topography::getSeed() {
    return seed;
}

void Topography::setSeed(unsigned char seed) {
    this->seed = seed;
}

int Topography::getType() {
    return type;
}

void Topography::setType(int type) {
    this->type = type;
}

float Topography::getScale() {
    return scale;
}

int Topography::getSize() {
    return size;
}

int Topography::getAmplitude() {
    return amplitude;
}

void Topography::setScale(float scale) {
    this->scale = scale;
}

float Topography::getSpacing() {
    return spacing;
}
