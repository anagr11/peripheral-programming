#pragma once

#include "gl/Camera.hpp"
#include "gl/Window.hpp"

#include <glm/glm.hpp>

extern gl::Window window;
extern gl::Camera camera;
extern glm::mat4 mat_persp, mat_view;
extern const float fov;
extern float deltaTime;

int main();
