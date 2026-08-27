#include "planet.hpp"

#include <cmath>

#include "math.hpp"

using std::sqrt, std::pow;

// Generate a cube face
MeshData generateCubeFace(int resolution) {
    MeshData data;

    for (int row = 0; row < resolution; row++) {
        for (int col = 0; col < resolution; col++) {
            float x = (col / ((float)resolution - 1)) * 2 - 1;
            float y = (row / ((float)resolution - 1)) * 2 - 1;
            float z = 1.0f;

            // Calculate warped values
            float xw = warpToSphere(y, z, x);
            float yw = warpToSphere(x, z, y);
            float zw = warpToSphere(x, y, z);

            float u = col / ((float)resolution - 1);
            float v = row / ((float)resolution - 1);

            data.vertices.push_back(xw);
            data.vertices.push_back(yw);
            data.vertices.push_back(zw);
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
Mesh joinMesh(int res) {
    MeshData data = generateCubeFace(res);
    Mesh mesh = createMesh(data.vertices, data.indices);
    return mesh;
}