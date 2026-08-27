#include "planet.hpp"

#include <iostream>

#include <glm/glm.hpp>

#include "math.hpp"
#include "../core/constants.hpp"

// Using declarations for std
using std::cerr, std::endl;
using std::array;

// Using declarations for glm
using glm::vec3;

// Generate a spheroid face with noise
MeshData generateSpheroidFace(int resolution, const FastNoiseLite& noise, float noiseScale, float heightScale, int faceIndex) {
    MeshData data;

    array<array<vec3, 3>, 6> faceVectors = {{
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

// Helper to join meshes with their data
Mesh joinMeshSpheroidFace(int res, const FastNoiseLite& noise, float noiseScale, float heightScale, int faceIndex) {
    MeshData data = generateSpheroidFace(res, noise, noiseScale, heightScale, faceIndex);
    Mesh mesh = createMesh(data.vertices, data.indices);
    return mesh;
}

array<Mesh, 6> createPlanetMeshGroup(int res, const FastNoiseLite& noise, float noiseScale, float heightScale) {
    array<Mesh, 6> faces = {};
    for (int i = 0; i < 6; i++) {
        faces.at(i) = joinMeshSpheroidFace(res, noise, noiseScale, heightScale, i);
    }
    return faces;
}