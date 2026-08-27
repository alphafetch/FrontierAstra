#include "camera.hpp"

#include <algorithm>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../core/constants.hpp"

// Using declarations for std
using std::clamp;

// Using declarations for glm
using glm::vec3, glm::radians, glm::cos, 
      glm::sin, glm::normalize, glm::cross;

// Callback for mouse movement
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    // Set current pos to previous if this is the first frame
    if (cam->firstMouse) {
        cam->lastX = (float)xpos;
        cam->lastY = (float)ypos;
        cam->firstMouse = false;
        return;
    }

    // Calculate delta x/y
    float deltaX = (float)xpos - cam->lastX;
    float deltaY = (float)ypos - cam->lastY;
    cam->lastX = (float)xpos;
    cam->lastY = (float)ypos;

    // Set the new pitch and yaw
    cam->setYaw(cam->yaw + deltaX * SENSITIVITY);
    cam->setPitch(cam->pitch - deltaY * SENSITIVITY);

    // Clamp the pitch
    float new_pitch = std::clamp(cam->pitch, LOW_PITCH_CLAMP, HIGH_PITCH_CLAMP);
    cam->setPitch(new_pitch);

    // Recompute front and right vectors
    cam->front.x = cos(cam->yawRad) * cos(cam->pitchRad);
    cam->front.y = sin(cam->pitchRad);
    cam->front.z = sin(cam->yawRad) * cos(cam->pitchRad);
    cam->front = normalize(cam->front);

    cam->right = normalize(cross(cam->front, cam->up));
}

// Scan for input across WASD
void processWASD(GLFWwindow* window, Camera& cam, float frameTime) {
    // WASD movement
    vec3 moveDir(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) moveDir += cam.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) moveDir -= cam.front;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) moveDir += cam.right;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) moveDir -= cam.right;

    if (moveDir != vec3(0.0f)) {
        moveDir = normalize(moveDir);
        cam.position += moveDir * SPEED * frameTime;
    }
}