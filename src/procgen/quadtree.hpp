#ifndef PROCGEN_QUADTREE_HPP
#define PROCGEN_QUADTREE_HPP

#include <array>
#include <memory>

#include <glm/glm.hpp>

struct QuadNode {
    int faceIndex;
    glm::vec3 center;
    float size;
    size_t depth;

    std::array<std::unique_ptr<QuadNode>, 4> children;

    QuadNode(int faceIndex, glm::vec3 center, float size, size_t depth)
        : faceIndex(faceIndex), center(center), size(size), depth(depth) {}

    void subdivideCheck(const glm::vec3& cameraPos, float distFactor, size_t maxDepth);
    void subdivide();
};

#endif