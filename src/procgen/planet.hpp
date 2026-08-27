#ifndef PROCGEN_PLANET_HPP
#define PROCGEN_PLANET_HPP

#include <vector>

#include <fastnoise/FastNoiseLite.h>

#include "../render/mesh.hpp"

// Generate a cube face
MeshData generateCubeFace(int resolution, const FastNoiseLite& noise, float noiseScale, float heightScale);

// Helper to join meshes with their data
Mesh joinMesh(int res, const FastNoiseLite& noise, float noiseScale, float heightScale);

#endif