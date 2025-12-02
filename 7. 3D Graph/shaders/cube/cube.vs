#version 330 core

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec2 v_tex;

out vec3 f_pos;
out vec2 f_tex;

uniform mat4 matrix;

void main() {
    f_pos = v_pos;
    f_tex = v_tex;
    gl_Position = matrix * vec4(v_pos, 1.0);
}
