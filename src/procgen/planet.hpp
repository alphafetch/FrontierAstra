#ifndef PROCGEN_PLANET_HPP
#define PROCGEN_PLANET_HPP

#include <vector>
#include <array>

#include <fastnoise/FastNoiseLite.h>

#include "../render/mesh.hpp"
#include "../core/entity.hpp"

// Generate a spheroid face with noise
MeshData generateSpheroidFace(int resolution, const FastNoiseLite& noise, float noiseScale, float heightScale, int faceIndex);

// Helper to create an array of MeshData structs from the function above
std::array<MeshData, 6> createPlanetMeshDataGroup(int res, const FastNoiseLite& noise, float noiseScale, float heightScale);

// Merge the data from the function above
MeshData mergeSpheroidFaceData(std::array<MeshData, 6> data);

// Link all these functions together
Mesh createPlanet(int res, const FastNoiseLite& noise, float noiseScale, float heightScale);

#endif