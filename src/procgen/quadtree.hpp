#ifndef PROCGEN_QUADTREE_HPP
#define PROCGEN_QUADTREE_HPP

#include <array>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <fastnoise/FastNoiseLite.h>

#include "../render/mesh.hpp"
#include "../render/camera.hpp"

struct QuadNode {
    int faceIndex;
    glm::vec3 center;
    float size;
    size_t depth;

    std::array<std::unique_ptr<QuadNode>, 4> children;

    QuadNode(int faceIndex, glm::vec3 center, float size, size_t depth)
        : faceIndex(faceIndex), center(center), size(size), depth(depth) {}

    void collectLeaves(std::vector<QuadNode*>& outLeaves);
    void subdivideCheck(const glm::vec3& cameraPos, float distFactor, size_t maxDepth);
    void subdivide();
};

// CPU side mesh generation
MeshData generateLODPlanetMeshData(Camera cam, float distFactor, int maxDepth, const FastNoiseLite& noise, float noiseScale, float heightScale, int leafRes);

// Generate the mesh with QuadNodes
Mesh generateLODPlanetMesh(Camera cam, float distFactor, int maxDepth, const FastNoiseLite& noise, float noiseScale, float heightScale, int leafRes);

#endif