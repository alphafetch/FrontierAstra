#ifndef PROCGEN_PLANET_HPP
#define PROCGEN_PLANET_HPP

#include <vector>
#include <array>
#include <string>

#include <fastnoise/FastNoiseLite.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "../render/mesh.hpp"
#include "../core/entity.hpp"
#include "../render/resource_manager.hpp"

// Generate a spheroid face with noise
MeshData generateSpheroidFace(int resolution, const FastNoiseLite& noise, float noiseScale, float heightScale, int faceIndex);

// Helper to create an array of MeshData structs from the function above
std::array<MeshData, 6> createPlanetMeshDataGroup(int res, const FastNoiseLite& noise, float noiseScale, float heightScale);

// Merge the data from the function above
MeshData mergeSpheroidFaceData(std::array<MeshData, 6> data);

// Link all these functions together to create a unified mesh
Mesh createPlanetMesh(int res, const FastNoiseLite& noise, float noiseScale, float heightScale);

// Link all creation functions together
entt::entity createPlanet(Mesh mesh, entt::registry& reg, ResourceManager<GLuint> texManager, ResourceManager<GLuint> shaderManager, std::string texturePath, std::string shaderKey, glm::vec3 position);

#endif