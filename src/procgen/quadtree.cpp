#include "quadtree.hpp"

#include "math.hpp"
#include "common.hpp"
#include "../core/constants.hpp"

using glm::vec3;
using std::make_unique, std::unique_ptr;

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