#ifndef RENDER_CAMERA_HPP
#define RENDER_CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Camera struct
struct Camera {
    // Position and direction vectors
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 front;

    // Directional pitch and yaw
    float yaw, pitch;
    float yawRad, pitchRad;

    // Mouse movement storage
    float lastX, lastY;
    bool firstMouse = true;

    // Set functions
    void setYaw(float deg) {
        yaw = deg;
        yawRad = glm::radians(yaw);
    }

    void setPitch(float deg) {
        pitch = deg;
        pitchRad = glm::radians(pitch);
    }
};

// Callback for mouse movement
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

// Scan for input across WASD
void processWASD(GLFWwindow* window, Camera& cam, float frameTime);

// Check for movement
bool checkCameraMovement(Camera& cam, float distThreshold);

#endif