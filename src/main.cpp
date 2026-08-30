#include <iostream>
#include <cstdlib>
#include <vector>
#include <array>
#include <random>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
#include "procgen/quadtree.hpp"

// Using declarations for std
using std::cerr, std::exit, std::endl;
using std::string, std::vector, std::array;

// Using declarations for glm
using glm::vec3, glm::radians, glm::cos, 
      glm::sin, glm::normalize, glm::mat4,
      glm::cross, glm::lookAt, glm::value_ptr,
      glm::perspective, glm::dvec3;

// Using declarations for entt
using entt::registry, entt::entity;

int main() {
    // Random initializations
    std::mt19937 rng(MASTER_SEED);
    std::uniform_int_distribution<int> noiseScaleRange(
        PLANET_NOISE_SCALE_MIN,
        PLANET_NOISE_SCALE_MAX
    );
    std::uniform_real_distribution<float>  heightScaleRange(
        PLANET_HEIGHT_SCALE_MIN,
        PLANET_HEIGHT_SCALE_MAX
    );

    // Initialize window
    int windowInitSuccess = glfwInit();
    if (windowInitSuccess == 0) {
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

    // Create camera
    Camera cam;
    cam.truePosition = dvec3(0, 0, 3);
    cam.setYaw(-90.0f); 
    cam.setPitch(0.0f);
    cam.up = vec3(0, 1, 0);

    cam.front.x = cos(cam.yawRad) * cos(cam.pitchRad);
    cam.front.y = sin(cam.pitchRad);
    cam.front.z = sin(cam.yawRad) * cos(cam.pitchRad);

    cam.front = normalize(cam.front);
    cam.right = normalize(cross(cam.front, cam.up));

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
            int textureUploadSuccess = upload2DTexture(key);
            if (textureUploadSuccess == 1) {
                exit(EXIT_FAILURE);
            }

            return texture;
        }
    );

    // Create the shader program via shaderManager
    string basicShaderKey = createKey(BASIC_VERT_SHADER, BASIC_FRAG_SHADER);
    GLuint basicShaderProgram = shaderManager.get(basicShaderKey);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Planet instances
    vector<PlanetInstance> planets;

    // Set up the entity registry to hold each individual entity
    registry reg;

    // Set up the FastNoiseLite for noise generation
    FastNoiseLite planetNoise(getChildSeed(MASTER_SEED, 0));
    planetNoise.SetNoiseType(GLOBAL_NOISE_TYPE);
    planetNoise.SetFrequency(PLANET_FREQ);
    planetNoise.SetFractalType(GLOBAL_FRACTAL_TYPE);
    planetNoise.SetFractalOctaves(PLANET_OCTAVES);
    planetNoise.SetFractalLacunarity(PLANET_LACUNARITY);
    planetNoise.SetFractalGain(PLANET_FRACTAL_GAIN);

    // Create planets, and store them in `reg`
    float tempNoiseScale, tempHeightScale;
    array<dvec3, PLANET_COUNT> positions = {
        ZERO_DVEC3,
        dvec3(3, 0, 0)
    };
    
    for (int i = 0; i < PLANET_COUNT; i++) {
        tempNoiseScale = noiseScaleRange(rng);
        tempHeightScale = heightScaleRange(rng);
        planets.push_back(
            PlanetInstance(
                createPlanet(
                    generateLODPlanetMesh(
                        cam, 
                        PLANET_QUADNODE_DISTANCE_FACTOR, 
                        PLANET_QUADNODE_MAX_DEPTH,
                        planetNoise,
                        tempNoiseScale, tempHeightScale,
                        PLANET_QUADNODE_LEAF_RESOLUTION,
                        positions.at(i)
                    ),
                    reg, textureManager, shaderManager, 
                    CONTAINER_JPG_TEX, createKey(BASIC_VERT_SHADER, BASIC_FRAG_SHADER), 
                    positions.at(i)
                ),
                tempNoiseScale, tempHeightScale
            )
        );
    }

    // Setup mouse
    glfwSetWindowUserPointer(window, &cam);
    glfwSetCursorPosCallback(window, mouseCallback);

    // MVP space
    mat4 view = lookAt(vec3(0.0f), cam.front, cam.up);
    mat4 proj = perspective(radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_CLIP, FAR_CLIP);

    glUseProgram(basicShaderProgram);

    GLint modelLoc = glGetUniformLocation(basicShaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(basicShaderProgram, "view");
    GLint projLoc = glGetUniformLocation(basicShaderProgram, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(proj));

    glUniform1i(glGetUniformLocation(basicShaderProgram, "texture1"), 0);

    // Set last frame & other misc. vars
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
            gameLogic(
                cam, reg, planets, planetNoise,
                PLANET_QUADNODE_DISTANCE_FACTOR,
                PLANET_QUADNODE_MAX_DEPTH,
                PLANET_QUADNODE_LEAF_RESOLUTION
            );
            
            accumulator -= FIXED_TIME;
        }

        // Set camera direction based on current positions
        view = lookAt(vec3(0.0f), cam.front, cam.up);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));

        // Clear from the stale previous frame
        glClearColor(SCR_CLEAR_COLOR);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render to the screen
        renderModels(reg, modelLoc, cam);

        glfwSwapBuffers(window);
    }
}