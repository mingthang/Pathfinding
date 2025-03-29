#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Renderer/Types/Texture.h>

namespace OpenGLBackEnd {
	// Core
	void Init();
	void BeginFrame();

	// Textures
	void AllocateTextureMemory(Texture& texture);

	// GET
	GLuint GetVertexDataVAO();
	GLuint GetVertexDataVBO();
	GLuint GetVertexDataEBO();
}
