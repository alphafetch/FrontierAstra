#include "planet.hpp"

#include <iostream>
#include <cstdlib>

#include "math.hpp"
#include "common.hpp"
#include "../core/constants.hpp"

// Using declarations for std
using std::cerr, std::endl;
using std::array, std::string;

// Using declarations for glm
using glm::vec3;

// Using declarations for entt
using entt::entity, entt::registry;

using utilarrs::faceVectors;

// Generate a spheroid face with noise
MeshData generatePlanetFace(int res, const FastNoiseLite& noise, float noiseScale, float heightScale, int faceIndex, vec3 center, float size) {
    MeshData data;

    if (faceIndex < 0 || faceIndex > 5) {
        cerr << "Face index " << faceIndex << " not found." << endl;
        exit(EXIT_FAILURE);
    }

    vec3 normal = faceVectors[faceIndex][0];
    vec3 right = faceVectors[faceIndex][1];
    vec3 up = faceVectors[faceIndex][2];

    for (int row = 0; row < res; row++) {
        for (int col = 0; col < res; col++) {
            float colValue = center.x + ((col / (static_cast<float>(res) - 1)) * 2 - 1) * (size / 2);
            float rowValue = center.y + ((row / (static_cast<float>(res) - 1)) * 2 - 1) * (size / 2);

            vec3 pos = normal + (colValue * right) + (rowValue * up);

            // Calculate warped values
            vec3 warpedPos(
                warpToSphere(pos.y, pos.z, pos.x),
                warpToSphere(pos.x, pos.z, pos.y),
                warpToSphere(pos.x, pos.y, pos.z)
            );

            float u = col / (static_cast<float>(res) - 1);
            float v = row / (static_cast<float>(res) - 1);

            // Apply noise
            vec3 scaledPos(
                warpedPos.x * noiseScale,
                warpedPos.y * noiseScale,
                warpedPos.z * noiseScale
            );

            float height = heightScale * noise.GetNoise(scaledPos.x, scaledPos.y, scaledPos.z);
            warpedPos.x *= PLANET_BASE_RADIUS + height;
            warpedPos.y *= PLANET_BASE_RADIUS + height;
            warpedPos.z *= PLANET_BASE_RADIUS + height;

            data.vertices.push_back(warpedPos.x);
            data.vertices.push_back(warpedPos.y);
            data.vertices.push_back(warpedPos.z);
            data.vertices.push_back(u);
            data.vertices.push_back(v);
        }
    }

    for (int row = 0; row < res - 1; row++) {
        for (int col = 0; col < res - 1; col++) {
            int topLeft = row * res + col;
            int topRight = row * res + (col + 1);
            int bottomLeft = (row + 1) * res + col;
            int bottomRight = (row + 1) * res + (col + 1);

            data.indices.push_back(topLeft);
            data.indices.push_back(bottomLeft);
            data.indices.push_back(topRight);

            data.indices.push_back(topRight);
            data.indices.push_back(bottomLeft);
            data.indices.push_back(bottomRight);
        }
    }

    return data;
}

// Helper to create an array of MeshData structs from the function above
array<MeshData, 6> createPlanetMeshDataGroup(int res, const FastNoiseLite& noise, float noiseScale, float heightScale) {
    array<MeshData, 6> faces = {};
    for (int i = 0; i < 6; i++) {
        faces.at(i) = generatePlanetFace(res, noise, noiseScale, heightScale, i, vec3(0, 0, 0), PLANET_FACE_FULL_SIZE);
    }
    return faces;
}

// ref: Mesh mesh = createMesh(data.vertices, data.indices);
// Merge the data from the function above
MeshData mergePlanetFaceData(const array<MeshData, 6>& dataArr) {
    MeshData data;
    for (int i = 0; i < 6; i++) {
        data.vertices.insert(data.vertices.end(), dataArr[i].vertices.begin(), dataArr[i].vertices.end());
    }

    int vertOffset = 0;
    for (int i = 0; i < 6; i++) {
        const MeshData& current = dataArr.at(i);
        for (size_t j = 0; j < current.indices.size(); j++) {
            data.indices.push_back(current.indices.at(j) + vertOffset);
        }
        vertOffset += dataArr.at(i).vertices.size() / 5;
    }

    return data;
}

// Link all these functions together
Mesh createPlanetMesh(int res, const FastNoiseLite& noise, float noiseScale, float heightScale) {
    array<MeshData, 6> faces = createPlanetMeshDataGroup(res, noise, noiseScale, heightScale);
    MeshData merged = mergePlanetFaceData(faces);
    Mesh mesh = createMesh(merged.vertices, merged.indices);
    
    return mesh;
}

// Ease of use entity creator for planets
entity createPlanet(Mesh mesh, registry& reg, ResourceManager<GLuint>& textureManager, ResourceManager<GLuint>& shaderManager, const string& texturePath, const string& shaderKey, vec3 position) {
    entity planet = reg.create();
    reg.emplace<Model>(planet, mesh, textureManager.get(texturePath), shaderManager.get(shaderKey));
    reg.emplace<Transform>(planet, position);

    return planet;
}