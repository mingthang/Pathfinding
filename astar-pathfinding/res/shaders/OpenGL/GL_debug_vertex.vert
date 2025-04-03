#version 460 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in ivec2 vPixelOffset;
layout (location = 3) in int vExclusiveViewportIndex;

uniform int u_viewportIndex;
out vec3 Color;

void main() {
    Color = vColor;
	gl_Position = vec4(vPosition, 1.0);

    if (vExclusiveViewportIndex != -1 && vExclusiveViewportIndex != u_viewportIndex) {
       gl_Position = vec4(0,0,0, 0);
    }
}