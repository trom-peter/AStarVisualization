#pragma once
#include <glm/glm.hpp>
#include <functional>

struct State {
	State(const int x, const int y, const int z) : x(x), y(y), z(z) {}
	State() : x(0), y(0), z(0) {}

    bool operator==(const State& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const State& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    glm::ivec2 getXZ() const {
        return glm::ivec2(x, z);
    }

    void setXZ(const glm::ivec2 xz) {
        x = xz.x;
        z = xz.y;
    }

	int x;
    int y;
    int z;
};

struct StateHash {
    size_t operator()(const State& state) const {
        // Combine x y z hash
        size_t hx = std::hash<int>()(state.x);
        size_t hy = std::hash<int>()(state.y);
        size_t hz = std::hash<int>()(state.z);
        return (hx ^ (hy << 1) ^ hz);
    }
};

