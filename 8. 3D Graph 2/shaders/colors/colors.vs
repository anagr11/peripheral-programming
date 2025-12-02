#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vColor;
out vec2 vPos;

uniform mat4 matrix;

void main() {
    vColor = aColor;
    gl_Position = matrix * vec4(aPos, 1.0);
}
