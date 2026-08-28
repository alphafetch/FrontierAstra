#include "planet.hpp"

#include <iostream>

#include "math.hpp"
#include "../core/constants.hpp"
#include "../utils/utils.hpp"

// Using declarations for std
using std::cerr, std::endl;
using std::array, std::string;

// Using declarations for glm
using glm::vec3;

// Using declarations for entt
using entt::entity, entt::registry;

// Generate a spheroid face with noise
MeshData generateSpheroidFace(int resolution, const FastNoiseLite& noise, float noiseScale, float heightScale, int faceIndex) {
    MeshData data;

    static const array<array<vec3, 3>, 6> faceVectors = {{
        // Normal      | Right         | Up
        {vec3(1, 0, 0),  vec3(0, 0, -1),vec3(0, 1, 0)}, // +X - 0i
        {vec3(-1, 0, 0), vec3(0, 0, 1), vec3(0, 1, 0)}, // -X - 1i
        {vec3(0, 1, 0),  vec3(1, 0, 0), vec3(0, 0, -1)},// +Y - 2i
        {vec3(0, -1, 0), vec3(1, 0, 0), vec3(0, 0, 1)}, // -Y - 3i
        {vec3(0, 0, 1),  vec3(1, 0, 0), vec3(0, 1, 0)}, // +Z - 4i
        {vec3(0, 0, -1), vec3(-1, 0, 0),vec3(0, 1, 0)}  // -Z - 5i
    }};

    if (faceIndex < 0 || faceIndex > 5) {
        cerr << "Face index " << faceIndex << " not found." << endl;
        exit(EXIT_FAILURE);
    }

    vec3 normal = faceVectors[faceIndex][0];
    vec3 right = faceVectors[faceIndex][1];
    vec3 up = faceVectors[faceIndex][2];

    for (int row = 0; row < resolution; row++) {
        for (int col = 0; col < resolution; col++) {
            float colValue = (col / (static_cast<float>(resolution) - 1)) * 2 - 1;
            float rowValue = (row / (static_cast<float>(resolution) - 1)) * 2 - 1;

            vec3 pos = normal + (colValue * right) + (rowValue * up);

            // Calculate warped values
            vec3 warpedPos(
                warpToSphere(pos.y, pos.z, pos.x),
                warpToSphere(pos.x, pos.z, pos.y),
                warpToSphere(pos.x, pos.y, pos.z)
            );

            float u = col / (static_cast<float>(resolution) - 1);
            float v = row / (static_cast<float>(resolution) - 1);

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

    for (int row = 0; row < resolution - 1; row++) {
        for (int col = 0; col < resolution - 1; col++) {
            int topLeft = row * resolution + col;
            int topRight = row * resolution + (col + 1);
            int bottomLeft = (row + 1) * resolution + col;
            int bottomRight = (row + 1) * resolution + (col + 1);

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
        faces.at(i) = generateSpheroidFace(res, noise, noiseScale, heightScale, i);
    }
    return faces;
}

// ref: Mesh mesh = createMesh(data.vertices, data.indices);
// Merge the data from the function above
MeshData mergeSpheroidFaceData(array<MeshData, 6>& dataArr) {
    MeshData data;
    for (int i = 0; i < 6; i++) {
        data.vertices.insert(data.vertices.end(), dataArr[i].vertices.begin(), dataArr[i].vertices.end());
    }

    int vertOffset = 0;
    MeshData current;
    for (int i = 0; i < 6; i++) {
        current = dataArr.at(i);
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
    MeshData merged = mergeSpheroidFaceData(faces);
    Mesh mesh = createMesh(merged.vertices, merged.indices);
    
    return mesh;
}

// Ease of use entity creator for planets
entity createPlanet(Mesh mesh, registry& reg, ResourceManager<GLuint>& textureManager, ResourceManager<GLuint>& shaderManager, string texturePath, string shaderKey, vec3 position) {
    entity planet = reg.create();
    reg.emplace<Model>(planet, mesh, textureManager.get(texturePath), shaderManager.get(shaderKey));
    reg.emplace<Transform>(planet, position);

    return planet;
}