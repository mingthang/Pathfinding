#version 460 core

layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec4 vColor;

out vec4 Color;
out vec2 TexCoord;

uniform mat4 u_modelMatrix;

void main() {
    Color = vColor;
    TexCoord = vUV;
    gl_Position = u_modelMatrix * vec4(vPosition, 0.0, 1.0);
}