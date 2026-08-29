#ifndef PROCGEN_PLANET_HPP
#define PROCGEN_PLANET_HPP

#include <vector>
#include <string>
#include <future>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <fastnoise/FastNoiseLite.h>

#include "../render/mesh.hpp"
#include "../render/resource_manager.hpp"

struct PlanetInstance {
    entt::entity entity;
    float noiseScale;
    float heightScale;

    std::future<MeshData> pendingData;

    PlanetInstance(entt::entity inst, float noiseScale, float heightScale)
        : entity(inst), noiseScale(noiseScale), heightScale(heightScale) {}
};

// Generate a spheroid face with noise
MeshData generatePlanetFace(int res, const FastNoiseLite& noise, float noiseScale, float heightScale, int faceIndex, glm::vec3 center, float size);

// Merge the data from the function above
MeshData mergePlanetFaceData(const std::vector<MeshData>& dataVect);

// Link all creation functions together
entt::entity createPlanet(Mesh mesh, entt::registry& reg, ResourceManager<GLuint>& textureManager, ResourceManager<GLuint>& shaderManager, const std::string& texturePath, const std::string& shaderKey, glm::vec3 position);

#endif