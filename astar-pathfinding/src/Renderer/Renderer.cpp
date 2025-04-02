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

    void DrawPoint(glm::vec3 position, glm::vec3 color, bool obeyDepth, int exclusiveViewportIndex) {
        OpenGLRenderer::DrawPoint(position, color, obeyDepth, exclusiveViewportIndex);
    }

    void DrawLine(glm::vec3 begin, glm::vec3 end, glm::vec3 color, bool obeyDepth, int exclusiveViewportIndex) {
        OpenGLRenderer::DrawLine(begin, end, color, obeyDepth, exclusiveViewportIndex);
    }
}