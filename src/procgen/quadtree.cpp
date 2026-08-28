#include "quadtree.hpp"

#include <vector>

#include <fastnoise/FastNoiseLite.h>

#include "math.hpp"
#include "common.hpp"
#include "../core/constants.hpp"
#include "planet.hpp"

using glm::vec3;

using std::make_unique, std::unique_ptr;
using std::vector;

using utilarrs::faceVectors, utilarrs::diagonalVectors;

// Check for a subdivide
void QuadNode::subdivideCheck(const vec3& cameraPos, float distFactor, size_t maxDepth) {
    vec3 normal = faceVectors[faceIndex][0];
    vec3 right = faceVectors[faceIndex][1];
    vec3 up = faceVectors[faceIndex][2];

    vec3 flatPos = normal + (center.x * right) + (center.y * up);
    vec3 worldPos(
        warpToSphere(flatPos.y, flatPos.z, flatPos.x),
        warpToSphere(flatPos.x, flatPos.z, flatPos.y),
        warpToSphere(flatPos.x, flatPos.y, flatPos.z)
    );
    worldPos *= PLANET_BASE_RADIUS;

    float dist = glm::distance(cameraPos, worldPos);

    if (dist < size * distFactor && depth < maxDepth) {
        if (!children[0]) {
            subdivide();
        }

        for (int i = 0; i < 4; i++) {
            children[i]->subdivideCheck(cameraPos, distFactor, maxDepth);
        }
    }
}

// Subdivide the face into four parts
void QuadNode::subdivide() {
    for (int i = 0; i < 4; i++) {
        vec3 quadrant = diagonalVectors.at(i);
        this->children[i] = make_unique<QuadNode>(
            this->faceIndex,
            this->center + quadrant * (this->size / 4),
            this->size / 2,
            this->depth + 1
        );
    }
}

// Collect the leaves from output
void QuadNode::collectLeaves(std::vector<QuadNode*>& outLeaves) {
    if (!children[0]) {
        outLeaves.push_back(this);
        return;
    }

    for (int i = 0; i < 4; i++) {
        children[i]->collectLeaves(outLeaves);
    }
}

Mesh generateLODPlanetMesh(Camera cam, float distFactor, int maxDepth, const FastNoiseLite& noise, float noiseScale, float heightScale, int leafRes) {
    vector<unique_ptr<QuadNode>> roots;
    for (int i = 0; i < 6; i++) {
        roots.push_back(make_unique<QuadNode>(
            i, ZERO_VEC3, 
            PLANET_FACE_FULL_SIZE, 0
        ));
    }

    for (const auto& i : roots) {
        i->subdivideCheck(cam.position, distFactor, maxDepth);
    }

    vector<QuadNode*> leaves;
    for (const auto& i : roots) {
        i->collectLeaves(leaves);
    }

    vector<MeshData> faceData;
    for (const auto& leaf : leaves) {
        MeshData leafData = generatePlanetFace(leafRes, noise, noiseScale, heightScale, leaf->faceIndex, leaf->center, leaf->size);
        faceData.push_back(leafData);
    }

    MeshData data = mergePlanetFaceData(faceData);
    Mesh mesh = createMesh(data.vertices, data.indices);

    return mesh;
}