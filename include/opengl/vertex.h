#pragma once

#include <vector>
#include <array>
#include "glm/glm.hpp"

enum class FaceDirection {
    FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;

    Vertex() : position(glm::vec3(0.0f)), normal(glm::vec3(0.0f)) {}

	Vertex(glm::vec3 position, glm::vec3 normal) : 
        position(position), normal(normal) {}

    static std::array<uint32_t, 6> getRectIndices() {
        return {
            0, 1, 2,
            2, 3, 0
        };
    }

    static std::vector<Vertex> getCubeVertices() {
        return {
            // (front face)
            Vertex(glm::vec3(0.0f, 0.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3(0.0f,  1.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),

            // (back face)
            Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            Vertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)) ,
            Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)) ,
            Vertex(glm::vec3(0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),

            // (left face)
            Vertex(glm::vec3(0.0f, 0.0f,  1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.0f,  1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.0f,  1.0f,  1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),

            // (right face)
            Vertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),

            // (top face)
            Vertex(glm::vec3(0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec3(0.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),

            // (bottom face)
            Vertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.0f,  1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            Vertex(glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
        };
    }

    static std::vector<uint32_t> getCubeIndices() {
        return {
            // Vorderseite (front face) - CCW
            0, 1, 2,
            0, 2, 3,

            // Rückseite (back face) - CCW
            4, 7, 6,
            6, 5, 4,

            // Linke Seite (left face) - CCW
            8, 11, 10,
            10, 9, 8,

            // Rechte Seite (right face) - CCW
            12, 15, 14,
            14, 13, 12,

            // Obere Seite (top face) - CCW
            16, 19, 18,
            18, 17, 16,

            // Untere Seite (bottom face) - CCW
            20, 22, 21,
            20, 23, 22
        };
    }

    static float calcVertexAO(bool side1, bool side2, bool corner) {
        if (side1 && side2) return 0.0f;
        return 3.0f - (side1 + side2 + corner);
    }
};