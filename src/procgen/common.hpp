#ifndef PROCGEN_COMMON_HPP
#define PROCGEN_COMMON_HPP

#include <array>

#include <glm/glm.hpp>

namespace utilarrs {
    inline constexpr std::array<std::array<glm::vec3, 3>, 6> faceVectors = {{
        // Normal      | Right         | Up
        {glm::vec3(1, 0, 0),  glm::vec3(0, 0, -1),glm::vec3(0, 1, 0)}, // +X - 0i
        {glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)}, // -X - 1i
        {glm::vec3(0, 1, 0),  glm::vec3(1, 0, 0), glm::vec3(0, 0, -1)},// +Y - 2i
        {glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)}, // -Y - 3i
        {glm::vec3(0, 0, 1),  glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)}, // +Z - 4i
        {glm::vec3(0, 0, -1), glm::vec3(-1, 0, 0),glm::vec3(0, 1, 0)}  // -Z - 5i
    }};

    inline constexpr std::array<glm::vec3, 4> diagonalVectors = {
        glm::vec3(-1, -1, 0), // Top left - 0
        glm::vec3(-1, 1, 0), // Bottom left - 1
        glm::vec3(1, -1, 0), // Top right - 2
        glm::vec3(1, 1, 0), // Bottom right - 3
    };
}

#endif