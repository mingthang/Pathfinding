#include "BackEnd.h"
#include "GLFWIntegration.h"
#include <API/OpenGL/GL_backEnd.h>
#include <API/OpenGL/GL_renderer.h>

namespace BackEnd {

	int g_presentTargetWidth = 0;
	int g_presentTargetHeight = 0;

	// Core
	bool Init(WindowedMode windowedMode) {
		if (!GLFWIntegration::Init(windowedMode)) {
			return false;
		}

		// OpenGL Backend
		OpenGLBackEnd::Init();
		OpenGLRenderer::Init();
	}

	void BeginFrame() {
		GLFWIntegration::BeginFrame();
	}

	void EndFrame() {
		GLFWIntegration::EndFrame();
	}

	bool WindowIsOpen() {
		return GLFWIntegration::WindowIsOpen();
	}

	void CleanUp() {
		GLFWIntegration::Destroy();
	}
}