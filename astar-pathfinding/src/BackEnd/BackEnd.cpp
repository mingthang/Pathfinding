#include "BackEnd.h"
#include "GLFWIntegration.h"
#include <API/OpenGL/GL_backEnd.h>
#include <API/OpenGL/Renderer/GL_renderer.h>
#include <AssetManager/AssetManager.h>
#include <UI/UIBackEnd.h>

namespace BackEnd {

	int g_presentTargetWidth = 0;
	int g_presentTargetHeight = 0;

	// Core
	bool Init(WindowedMode windowedMode) {
		if (!GLFWIntegration::Init(windowedMode)) {
			return false;
		}

		// OpenGL API
		OpenGLBackEnd::Init();
		OpenGLRenderer::Init();

		AssetManager::Init();
		UIBackEnd::Init();

		// Init sub-systems

		glfwShowWindow(static_cast<GLFWwindow*>(BackEnd::GetWindowPointer()));
		return true;
	}

	void BeginFrame() {
		GLFWIntegration::BeginFrame();
		//RenderDataManager::BeginFrame();
		//OpenGLBackEnd::BeginFrame();
	}

	void EndFrame() {
		GLFWIntegration::EndFrame();
	}

	void UpdateSubSystems() {

	}

	void CleanUp() {
		GLFWIntegration::Destroy();
	}

	// Window
	const WindowedMode& GetWindowedMode() {
		return GLFWIntegration::GetWindowedMode();
	}

	void SetWindowedMode(const WindowedMode& windowedMode) {
		GLFWIntegration::SetWindowedMode(windowedMode);
	}

	void* GetWindowPointer() {
		return GLFWIntegration::GetWindowPointer();;
	}

	bool WindowIsOpen() {
		return GLFWIntegration::WindowIsOpen();
	}

	int GetWindowedWidth() {
		return GLFWIntegration::GetWindowedWidth();
	}

	int GetWindowedHeight() {
		return GLFWIntegration::GetWindowedHeight();
	}

	int GetCurrentWindowWidth() {
		return GLFWIntegration::GetCurrentWindowWidth();
	}

	int GetCurrentWindowHeight() {
		return GLFWIntegration::GetCurrentWindowHeight();
	}

	int GetFullScreenWidth() {
		return GLFWIntegration::GetFullScreenWidth();
	}

	int GetFullScreenHeight() {
		return GLFWIntegration::GetFullScreenHeight();
	}
}