#ifndef CORE_ENTITY_HPP
#define CORE_ENTITY_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "../render/mesh.hpp"

struct Model {
    Mesh mesh;
    GLuint texture;
    GLuint shaderProg;
};

struct Transform {
    glm::vec3 position;
};

#endif