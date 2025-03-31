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

	// Utils
	GLint CreateQuadVAO();
}
