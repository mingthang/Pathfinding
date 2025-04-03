#version 460 core

layout (location = 0) out vec4 FragOut;
layout (binding = 0) uniform sampler2D Texture;

in vec4 Color;
in vec2 TexCoord;

uniform vec4 u_color;

void main() {
    FragOut = texture(Texture, TexCoord);
    FragOut *= u_color;
}