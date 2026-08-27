#ifndef PROCGEN_PLANET_HPP
#define PROCGEN_PLANET_HPP

#include <vector>
#include <array>

#include <fastnoise/FastNoiseLite.h>

#include "../render/mesh.hpp"

// Generate a cube face
MeshData generateSpheroidFace(int resolution, const FastNoiseLite& noise, float noiseScale, float heightScale, int faceIndex);

// Helper to join spheroid planet face meshes with their data
Mesh joinMeshSpheroidFace(int res, const FastNoiseLite& noise, float noiseScale, float heightScale, int faceIndex);

// Helper to loop through the faces of a planet mesh group
std::array<Mesh, 6> createPlanetMeshGroup(int res, const FastNoiseLite& noise, float noiseScale, float heightScale);

#endif