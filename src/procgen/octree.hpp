#ifndef PROCGEN_OCTREE_HPP
#define PROCGEN_OCTREE_HPP

#include <memory>
#include <vector>
#include <array>

#include <glm/glm.hpp>
#include <entt/entt.hpp>

struct OctreeNode {
    glm::dvec3 lowBound;
    glm::dvec3 highBound;
    size_t depth;

    std::array<std::unique_ptr<OctreeNode>, 8> children;
    std::vector<entt::entity> objects;

    OctreeNode(glm::dvec3 low, glm::dvec3 high, size_t depth)
        : lowBound(low), highBound(high), depth(depth) {}

    void collectLeaves(std::vector<OctreeNode*>& outLeaves);
    void subdivideCheck(int maxObjects, int maxDepth, entt::registry& reg);
    void subdivide();
    // Insert an entity registry into an OctreeNode
    void insert(entt::registry& reg);
    void entityInsert(entt::entity ent, Transform& t, entt::registry& reg);
};

#endif