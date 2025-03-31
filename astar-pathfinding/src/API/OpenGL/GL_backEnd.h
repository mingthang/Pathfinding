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
	void UpdateTextureBaking();
	void ImmediateBake(QueuedTextureBake& queuedTextureBake);
	void AsyncBakeQueuedTextureBake(QueuedTextureBake& queuedTextureBake);
	void CleanUpBakingPBOs();
	const std::vector<GLuint64>& GetBindlessTextureIDs();

	// GET
	GLuint GetVertexDataVAO();
	GLuint GetVertexDataVBO();
	GLuint GetVertexDataEBO();
}
