#include "Controls.hpp"
#include <glm/glm.hpp>

// ------------------------------------------------------------
// Internal state for mouse control
// ------------------------------------------------------------
static bool firstMouse = true;
static double lastX = 0.0;
static double lastY = 0.0;
static bool mouseCaptured = false;
static bool escPressed = false;

// Mouse sensitivity
static float mouseSensitivity = 0.1f;
static float yaw = 0, pitch = 0;

// Movement speed
static float movementSpeed = 15.0f;


// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------


void setupControls() {
    auto handle = window.getHandle();
    glfwSetCursorPosCallback(handle, mouseLookCallback);
    glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);
    
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    float ratio = float(width) / float(height);
    mat_persp = glm::perspective((float)(fov * std::numbers::pi / 180.f), ratio, 0.001f, 1000.f);
}

void processControls() {
    handleKeyboardInput();

    // Apply PITCH here
    gl::Camera cam2 = camera;
    cam2.rotateX(pitch);
    mat_view = cam2.getMatrix();
}

// ------------------------------------------------------------
// Keyboard input
// ------------------------------------------------------------
void handleKeyboardInput() {
    float vel = movementSpeed * deltaTime;

    // WASD
    if (window.isPressed(GLFW_KEY_W))
        camera.move(glm::vec3(0, 0, vel));     // forward

    if (window.isPressed(GLFW_KEY_S))
        camera.move(glm::vec3(0, 0, -vel));    // backward

    if (window.isPressed(GLFW_KEY_A))
        camera.move(glm::vec3(-vel, 0, 0));    // left

    if (window.isPressed(GLFW_KEY_D))
        camera.move(glm::vec3(vel, 0, 0));     // right

    // Up/down
    if (window.isPressed(GLFW_KEY_SPACE))
        camera.move(glm::vec3(0, vel, 0));     // up

    if (window.isPressed(GLFW_KEY_F))
        camera.move(glm::vec3(0, -vel, 0));    // down

    // ESC toggles mouse capture
    bool esc = window.isPressed(GLFW_KEY_ESCAPE);
    if (esc && escPressed == false) {
        toggleMouseCapture();
        escPressed = true;
    }
    if(!esc) {
        escPressed = false;
    }
}


// ------------------------------------------------------------
// Mouse look
// ------------------------------------------------------------
void mouseLookCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!mouseCaptured)
        return;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    double dx = xpos - lastX;
    double dy = lastY - ypos;   // inverted y-axis

    lastX = xpos;
    lastY = ypos;

    yaw = static_cast<float>(dx) * mouseSensitivity * -1.f;

    pitch += static_cast<float>(dy) * mouseSensitivity;
    if(pitch > 90) pitch = 90;
    if(pitch < -90) pitch = -90;

    camera.rotateY(yaw);
}


// ------------------------------------------------------------
// Mouse capture
// ------------------------------------------------------------
void toggleMouseCapture() {
    mouseCaptured = !mouseCaptured;
    auto handle = window.getHandle();

    if (mouseCaptured) {
        glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouse = true;
    } else {
        glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
