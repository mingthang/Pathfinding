#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace OpenGLBackEnd {
	// Core
	void Init();
	GLuint GetVertexDataVAO();
	GLuint GetVertexDataVBO();
	GLuint GetVertexDataEBO();
}
