#pragma once
#include <glm/glm.hpp>

struct State {
	State(int x, int y, int z) : x(x), y(y), z(z) {}
	State() : x(0), y(0), z(0) {}

    bool operator==(const State& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const State& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    glm::ivec2 getXZ() {
        return glm::ivec2(x, z);
    }

    void setXZ(glm::ivec2 xz) {
        x = xz.x;
        z = xz.y;
    }

	int x;
    int y;
    int z;
};

namespace std {
    template <>
    struct hash<State> {
        size_t operator()(const State& state) const {
            // combine x y z hash
            size_t h1 = std::hash<int>()(state.x);
            size_t h2 = std::hash<int>()(state.y);
            size_t h3 = std::hash<int>()(state.z);
            //return h1 ^ (h2 << 1);
            return (h1 ^ (h2 << 1) ^ h3);
        }
    };
}
