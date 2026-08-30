#ifndef CORE_UPDATE_HPP
#define CORE_UPDATE_HPP

#include <entt/entt.hpp>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <fastnoise/FastNoiseLite.h>

#include "../render/camera.hpp"
#include "../procgen/planet.hpp"

// Render models onto the screen
void renderModels(entt::registry& reg, GLint modelLoc, Camera& cam);

// Game logic
void gameLogic(Camera& cam, entt::registry& reg, std::vector<PlanetInstance>& planets, 
               const FastNoiseLite& planetNoise, float distFactor, int maxDepth, int leafRes);

#endif