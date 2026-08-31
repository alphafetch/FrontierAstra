#include "octree.hpp"

#include "glm/glm.hpp"

#include "../core/constants.hpp"

// Using declarations for std
using std::vector;

// Using declarations for glm
using glm::dvec3;

// Using declarations for entt
using entt::registry, entt::entity;

void OctreeNode::collectLeaves(vector<OctreeNode*>& outLeaves) {
    if (this->children[0] == nullptr) {
        outLeaves.push_back(this);
    } else {
        for (int i = 0; i < 8; i++) { 
            children[i]->collectLeaves(outLeaves);
        }
    }
}

void OctreeNode::subdivideCheck(size_t maxObjects, size_t maxDepth, registry& reg) {
    if (this->objects.size() >= maxObjects \
        && depth < maxDepth) {
        subdivide();
        for (auto& ent : this->objects) {
            Transform& t = reg.get<Transform>(ent);
            this->entityInsert(ent, t, reg);
        }
        this->objects.clear();
    }
    return;
}

void OctreeNode::subdivide() {
    dvec3 mid = (this->lowBound + this->highBound) / 2.0;
    for (int i = 0; i < 8; i++) {
        dvec3 childLow;
        dvec3 childHigh;
        for (int a = 0; a < 3; a++) {
            if ((i >> a) & 1) {
                childLow[a] = mid[a];
                childHigh[a] = this->highBound[a];
            } else {
                childLow[a] = this->lowBound[a];
                childHigh[a] = mid[a];
            }
        }
        children[i] = std::make_unique<OctreeNode>(childLow, childHigh, this->depth + 1);
    }
}

// Insert an entity registry into an OctreeNode
void OctreeNode::insert(registry& reg) {
    reg.view<Transform>().each([this, &reg](entity ent, Transform& t) {
            this->entityInsert(ent, t, reg);
        }
    );
}

void OctreeNode::entityInsert(entity ent, Transform& t, registry& reg) {
    if (this->children[0] != NULL) {
        dvec3 mid = (this->lowBound + this->highBound) / 2.0;
        size_t index = (t.truePosition.x >= mid.x ? 1 : 0)
                     | (t.truePosition.y >= mid.y ? 2 : 0)
                     | (t.truePosition.z >= mid.z ? 4 : 0);
        auto& containerChild = this->children[index];
        containerChild->entityInsert(ent, t, reg);
        return;
    } else {
        this->objects.push_back(ent);
        subdivideCheck(OCTREE_MAX_OBJECTS, OCTREE_MAX_DEPTH, reg);
        return;
    }
}