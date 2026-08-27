#include "update.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity.hpp"

// Using declarations for entt
using entt::registry, entt::entity;

// Using declarations for glm
using glm::translate, glm::mat4, glm::value_ptr;

// Render models onto the screen
void renderModels(registry& reg, GLint modelLoc) {
    reg.view<Model, Transform>().each([modelLoc](Model& model, Transform& transform) {
        // Setup
        glUseProgram(model.shaderProg);
        glBindVertexArray(model.mesh.vao);

        // Texturing
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model.texture);

        // Capture modelLoc and use it to assemble to model matrix
        mat4 modelMatrix = translate(mat4(1.0f), transform.position);

        // Draw the model
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(modelMatrix));
        glDrawArrays(GL_TRIANGLES, 0, model.mesh.vertexCount);
    });
}