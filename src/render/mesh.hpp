#ifndef RENDER_MESH_HPP
#define RENDER_MESH_HPP

#include <vector>

#include <glad/gl.h>

struct Mesh {
    GLuint vao;
    int indexCount;
    int vertexCount;
};

struct MeshData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

// Create the mesh vao, vbo, and ebo
Mesh createMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices = {});

#endif