#pragma once

#include "main.hpp"

void setupControls();

void processControls();

// Must be called once per frame
void handleKeyboardInput();

// Mouse callback
void mouseLookCallback(GLFWwindow* window, double xpos, double ypos);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

// Capture/release mouse toggler
void toggleMouseCapture();
