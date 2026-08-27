#include <iostream>
#include <array>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "camera.hpp"
#include "constants.hpp"
#include "shader.hpp"
#include "resource_manager.hpp"
#include "utils.hpp"
#include "texture.hpp"

// Using declarations for std
using std::cerr, std::exit, std::endl;
using std::array, std::string;

// Using declarations for glm
using glm::vec3, glm::radians, glm::cos, 
      glm::sin, glm::normalize, glm::mat4,
      glm::cross;

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
            upload2DTexture(key);

            return texture;
        }
    );

    // Create cube vertices
    float h = 0.5f;

    array<float, 180> vertices = {
        // Back face (Z = -h)          // UV
        -h, -h, -h,  0.0f, 0.0f,
         h, -h, -h,  1.0f, 0.0f,
         h,  h, -h,  1.0f, 1.0f,
         h,  h, -h,  1.0f, 1.0f,
        -h,  h, -h,  0.0f, 1.0f,
        -h, -h, -h,  0.0f, 0.0f,

        // Front face (Z = h)
        -h, -h,  h,  0.0f, 0.0f,
         h, -h,  h,  1.0f, 0.0f,
         h,  h,  h,  1.0f, 1.0f,
         h,  h,  h,  1.0f, 1.0f,
        -h,  h,  h,  0.0f, 1.0f,
        -h, -h,  h,  0.0f, 0.0f,

        // Left face (X = -h)
        -h,  h,  h,  1.0f, 0.0f,
        -h,  h, -h,  1.0f, 1.0f,
        -h, -h, -h,  0.0f, 1.0f,
        -h, -h, -h,  0.0f, 1.0f,
        -h, -h,  h,  0.0f, 0.0f,
        -h,  h,  h,  1.0f, 0.0f,

        // Right face (X = h)
         h,  h,  h,  1.0f, 0.0f,
         h,  h, -h,  1.0f, 1.0f,
         h, -h, -h,  0.0f, 1.0f,
         h, -h, -h,  0.0f, 1.0f,
         h, -h,  h,  0.0f, 0.0f,
         h,  h,  h,  1.0f, 0.0f,

        // Bottom face (Y = -h)
        -h, -h, -h,  0.0f, 1.0f,
         h, -h, -h,  1.0f, 1.0f,
         h, -h,  h,  1.0f, 0.0f,
         h, -h,  h,  1.0f, 0.0f,
        -h, -h,  h,  0.0f, 0.0f,
        -h, -h, -h,  0.0f, 1.0f,

        // Top face (Y = h)
        -h,  h, -h,  0.0f, 1.0f,
         h,  h, -h,  1.0f, 1.0f,
         h,  h,  h,  1.0f, 0.0f,
         h,  h,  h,  1.0f, 0.0f,
        -h,  h,  h,  0.0f, 0.0f,
        -h,  h, -h,  0.0f, 1.0f
    };

    // Data for the GPU
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    string basicShaderKey = createKey(BASIC_VERT_SHADER, BASIC_FRAG_SHADER);
    GLuint basicShaderProgram = shaderManager.get(basicShaderKey);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint texture = textureManager.get(CONTAINER_JPG_TEX);

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
    mat4 model(1.0f);
    mat4 view = glm::lookAt(cam.position, cam.position + cam.front, cam.up);
    mat4 proj = glm::perspective(radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_CLIP, FAR_CLIP);

    glUseProgram(basicShaderProgram);

    GLint modelLoc = glGetUniformLocation(basicShaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(basicShaderProgram, "view");
    GLint projLoc = glGetUniformLocation(basicShaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    // Bind and uniform wiring
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(basicShaderProgram, "texture1"), 0);

    // Set last frame
    float lastFrame = 0.0f;

    // Main window update loop
    while(!glfwWindowShouldClose(window)) {
        // Frame updates
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Get frame updates
        glfwPollEvents();

        // WASD movement
        vec3 moveDir(0.0f);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) moveDir += cam.front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) moveDir -= cam.front;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) moveDir += cam.right;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) moveDir -= cam.right;

        if (moveDir != vec3(0.0f)) {
            moveDir = normalize(moveDir);
            cam.position += moveDir * SPEED * deltaTime;
        }

        // Set camera direction based on current positions
        view = glm::lookAt(cam.position, cam.position + cam.front, cam.up);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Update the screen
        glClearColor(SCR_CLEAR_COLOR);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(basicShaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);

        glfwSwapBuffers(window);
    }
}