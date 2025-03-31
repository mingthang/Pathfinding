#include "Renderer.h"
#include <API/OpenGL/Renderer/GL_renderer.h>
#include <Config/Config.h>
#include <unordered_map>

namespace Renderer {

	void InitMain() {
		OpenGLRenderer::InitMain();
	}

	void RenderLoadingScreen() {
		OpenGLRenderer::RenderLoadingScreen();
	}

	void RenderGame() {
		OpenGLRenderer::RenderGame();
	}
}