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

	// Buffers
	void UploadVertexData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	// GET
	GLuint GetVertexDataVAO();
	GLuint GetVertexDataVBO();
	GLuint GetVertexDataEBO();
}
