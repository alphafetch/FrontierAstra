#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "mesh.hpp"

struct Model {
    Mesh mesh;
    GLuint texture;
    GLuint shaderProg;
};

struct Transform {
    glm::vec3 position;
};

#endif