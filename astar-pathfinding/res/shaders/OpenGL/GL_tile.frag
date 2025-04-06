#version 460 core

layout (location = 0) out vec4 FragOut;
layout (binding = 0) uniform sampler2D Texture;

in vec2 TexCoord;
in vec4 InstanceColor;

void main() {
    FragOut = texture(Texture, TexCoord);
    FragOut *= InstanceColor;
}