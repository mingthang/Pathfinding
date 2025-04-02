#pragma once

#include <Renderer/RendererCommon.h>
#include <API/OpenGL/Types/GL_shader.h>
#include <API/OpenGL/Types/GL_framebuffer.h>

namespace OpenGLRenderer {
	void Init();
	void InitMain();
	void RenderLoadingScreen();
	void RenderGame();

	void HotloadShaders();
	void CreateRenderTargets(int presentWidth, int presentHeight);
	void PresentFinalImage();

	OpenGLFrameBuffer* GetFrameBuffer(const std::string& name);
	OpenGLShader* GetShader(const std::string& name);

	// Render Passes
	void UIPass();
	void DebugPass();
	void GridPass();

	// Drawing
	void MultiDrawIndirect(const std::vector<DrawIndexedIndirectCommand>& commands);
	void SplitMultiDrawIndirect(OpenGLShader* shader, const std::vector<DrawIndexedIndirectCommand>& commands);

	// Utils
	GLint CreateQuadVAO();
	RenderItem2D CreateRenderItem2D(const std::string& textureName, glm::ivec2 location, glm::ivec2 viewportSize, Alignment alignment, glm::vec3 colorTint = glm::vec3(1.0), glm::ivec2 size = glm::ivec2(-1, -1));
	void BlitFrameBuffer(OpenGLFrameBuffer* srcFrameBuffer, OpenGLFrameBuffer* dstFrameBuffer, const char* srcName, const char* dstName, GLbitfield mask, GLenum filter);
	void BlitToDefaultFrameBuffer(OpenGLFrameBuffer* srcFrameBuffer, const char* srcName, GLbitfield mask, GLenum filter);

	// Debug
	void UpdateDebugMesh();
	void DrawPoint(glm::vec3 position, glm::vec3 color, bool obeyDepth = false, int exclusiveViewportIndex = -1);
	void DrawLine(glm::vec3 begin, glm::vec3 end, glm::vec3 color, bool obeyDepth = false, int exclusiveViewportIndex = -1);
}
