#version 330 core

in vec3 f_pos;
in vec2 f_tex;

out vec4 o_color;

uniform vec4 u_solidColor;
uniform sampler2D u_sampler;
uniform int u_mode;

const int FLAG_SOLID   = 1;
const int FLAG_TEXTURE = 2;

void main() {
    // We first set the color to be all 1s
    o_color = vec4(1.0);

    // If texture is enabled, use it
    // to override the color
    if((u_mode & FLAG_TEXTURE) != 0) {
        o_color = texture(u_sampler, f_tex);
    }

    // Use color as a multiplicate,
    // will add itself...
    if((u_mode & FLAG_SOLID) != 0) {
        o_color *= u_solidColor;
    }

    // Process final color...
    //float steps = 8.0f;
    //o_color.rgb = floor(o_color.rgb * steps) / steps;
}
