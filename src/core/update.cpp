#include "update.hpp"

#include <future>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity.hpp"
#include "constants.hpp"
#include "../procgen/quadtree.hpp"

// Using declarations for std
using std::vector, std::future_status, std::chrono::seconds;

// Using declarations for entt
using entt::registry, entt::entity;

// Using declarations for glm
using glm::translate, glm::mat4, glm::value_ptr,
      glm::vec3, glm::dvec3;

// Render models onto the screen
void renderModels(registry& reg, GLint modelLoc, Camera& cam) {
    reg.view<Model, Transform>().each([modelLoc, cam](Model& model, Transform& transform) {
        // Setup
        glUseProgram(model.shaderProg);
        glBindVertexArray(model.mesh.vao);

        // Texturing
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model.texture);

        // Capture modelLoc and use it to assemble to model matrix
        dvec3 offset = transform.truePosition - cam.truePosition;
        mat4 modelMatrix = translate(mat4(1.0f), vec3(offset));

        // Draw the model
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(modelMatrix));
        if (model.mesh.indexCount > 0) { glDrawElements(GL_TRIANGLES, model.mesh.indexCount, GL_UNSIGNED_INT, 0); }
        else { glDrawArrays(GL_TRIANGLES, 0, model.mesh.vertexCount); }
    });
}

// Game logic
void gameLogic(Camera& cam, registry& reg, vector<PlanetInstance>& planets, 
               const FastNoiseLite& planetNoise, float distFactor, int maxDepth, int leafRes) {
    bool movementThresholdReached = checkCameraMovement(cam, GLOBAL_LOD_REFRESH_DIST);
    for (auto& planet : planets) {
        if (planet.pendingData.valid() == true) {
            future_status status = planet.pendingData.wait_for(std::chrono::seconds(0));
            if (status == future_status::ready) {
                Model& model = reg.get<Model>(planet.entity);
                Mesh oldMesh = model.mesh;

                MeshData data = planet.pendingData.get();
                Mesh newMesh = createMesh(data.vertices, data.indices);
                model.mesh = newMesh;

                destroyMesh(oldMesh);
            }
        } else if (movementThresholdReached) {
            planet.pendingData = std::async(std::launch::async, generateLODPlanetMeshData, cam, 
                                            distFactor, maxDepth, planetNoise, planet.noiseScale, planet.heightScale, leafRes, reg.get<Transform>(planet.entity).truePosition);
        }
    }
}