#include <iostream>
#include <array>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <entt/entt.hpp>
#include <fastnoise/FastNoiseLite.h>

#include "render/camera.hpp"
#include "render/shader.hpp"
#include "render/resource_manager.hpp"
#include "render/texture.hpp"
#include "render/mesh.hpp"

#include "core/constants.hpp"
#include "core/entity.hpp"
#include "core/update.hpp"

#include "utils/utils.hpp"

#include "procgen/planet.hpp"
#include "procgen/math.hpp"

// Using declarations for std
using std::cerr, std::exit, std::endl;
using std::array, std::string;

// Using declarations for glm
using glm::vec3, glm::radians, glm::cos, 
      glm::sin, glm::normalize, glm::mat4,
      glm::cross, glm::lookAt, glm::value_ptr,
      glm::perspective;

// Using declarations for entt
using entt::registry, entt::entity;

int main() {
    // Initialize window
    int s = glfwInit();
    if (s == 0) {
        cerr << "glfwInit failed" << endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WIN_TITLE, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGL((GLADloadfunc) glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

    // Initialize the shader manager to compile and link shaders
    ResourceManager<GLuint> shaderManager(
        [](const std::string& key) -> GLuint {
            auto shader_pair = splitKey(key);
            string vertexShaderSource = readFile(shader_pair.first);
            string fragShaderSource = readFile(shader_pair.second);

            GLuint vertShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
            GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, fragShaderSource.c_str());

            GLuint shaderProgram = createShaderProgram(vertShader, fragShader);
            return shaderProgram;
        }
    );

    // Initialize the texture manager to load textures
    ResourceManager<GLuint> textureManager(
        [](const std::string& key) -> GLuint {
            auto texture = generate2DTexture();
            int s = upload2DTexture(key);
            if (s == 1) {
                exit(EXIT_FAILURE);
            }

            return texture;
        }
    );

    // Create the shader program via shaderManager
    string basicShaderKey = createKey(BASIC_VERT_SHADER, BASIC_FRAG_SHADER);
    GLuint basicShaderProgram = shaderManager.get(basicShaderKey);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set up the entity registry
    registry reg;

    // Set up the FastNoiseLite
    FastNoiseLite planetNoise(getChildSeed(MASTER_SEED, 0));
    planetNoise.SetNoiseType(GLOBAL_NOISE_TYPE);
    planetNoise.SetFrequency(PLANET_FREQ);
    planetNoise.SetFractalType(GLOBAL_FRACTAL_TYPE);
    planetNoise.SetFractalOctaves(PLANET_OCTAVES);
    planetNoise.SetFractalLacunarity(PLANET_LACUNARITY);
    planetNoise.SetFractalGain(PLANET_FRACTAL_GAIN);

    array<Mesh, 6> faceMeshes = createPlanetMeshGroup(128, planetNoise, 75, 0.10f);
    array<entity, 6> faceEntities = {};
    for (int i = 0; i < 6; i++) {
        faceEntities.at(i) = reg.create();
        reg.emplace<Model>(faceEntities[i], faceMeshes[i], textureManager.get(CONTAINER_JPG_TEX), shaderManager.get(createKey(BASIC_VERT_SHADER, BASIC_FRAG_SHADER)));
        reg.emplace<Transform>(faceEntities[i], ZERO_VEC3);
    }

    // Create camera
    Camera cam;
    cam.position = vec3(0, 0, 3);
    cam.setYaw(-90.0f); 
    cam.setPitch(0.0f);
    cam.up = vec3(0, 1, 0);

    cam.front.x = cos(cam.yawRad) * cos(cam.pitchRad);
    cam.front.y = sin(cam.pitchRad);
    cam.front.z = sin(cam.yawRad) * cos(cam.pitchRad);

    cam.front = normalize(cam.front);
    cam.right = normalize(cross(cam.front, cam.up));

    // Setup mouse
    glfwSetWindowUserPointer(window, &cam);
    glfwSetCursorPosCallback(window, mouseCallback);

    // MVP space
    mat4 view = lookAt(cam.position, cam.position + cam.front, cam.up);
    mat4 proj = perspective(radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_CLIP, FAR_CLIP);

    glUseProgram(basicShaderProgram);

    GLint modelLoc = glGetUniformLocation(basicShaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(basicShaderProgram, "view");
    GLint projLoc = glGetUniformLocation(basicShaderProgram, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(proj));

    glUniform1i(glGetUniformLocation(basicShaderProgram, "texture1"), 0);

    // Set last frame
    float lastFrame = 0.0f;
    float accumulator = 0.0f;

    // Main window update loop
    while(!glfwWindowShouldClose(window)) {
        // Frame updates and variables
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Get frame updates
        glfwPollEvents();
        
        // Get keyboard input for free-fly
        processWASD(window, cam, deltaTime);

        // Process logic
        accumulator += deltaTime;
        while (accumulator >= FIXED_TIME) {
            // game logic here
            accumulator -= FIXED_TIME;
        }

        // Set camera direction based on current positions
        view = lookAt(cam.position, cam.position + cam.front, cam.up);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));

        // Clear from the stale previous frame
        glClearColor(SCR_CLEAR_COLOR);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render to the screen
        renderModels(reg, modelLoc);

        glfwSwapBuffers(window);
    }
}