#version 460 core

layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec4 vTangent;
// Instance matrix
layout(location = 4) in mat4 i_ModelMatrix;
// Instance color
layout(location = 8) in vec4 i_Color;

out vec2 TexCoord;
out vec4 InstanceColor;

void main() {
    TexCoord = vUV;
    InstanceColor = i_Color;
    gl_Position = i_ModelMatrix * vec4(vPosition, 0.0, 1.0);
}