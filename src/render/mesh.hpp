#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include <glad/gl.h>

struct Mesh {
    GLuint vao;
    int vertexCount;
};

GLuint createMeshVAO(const std::vector<float>& vertices);

#endif