#ifndef PROCGEN_PLANET_HPP
#define PROCGEN_PLANET_HPP

#include <vector>

#include "../render/mesh.hpp"

// Generate a cube face
MeshData generateCubeFace(int resolution);

// Helper to join meshes with their data
Mesh joinMesh(int res);

#endif