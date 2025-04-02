#include "BackEnd.h"
#include "GLFWIntegration.h"
#include <API/OpenGL/GL_backEnd.h>
#include <API/OpenGL/Renderer/GL_renderer.h>
#include <AssetManager/AssetManager.h>
#include <UI/UIBackEnd.h>
#include <Core/Input/Input.h>
#include <Core/Debug.h>
#include <Core/Audio/Audio.h>
#include <Core/Game.h>
#include <Config/Config.h>
#include <Common/EngineDefines.h>
#include <Renderer/RenderDataManager.h>

#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif

namespace BackEnd {

	int g_presentTargetWidth = 0;
	int g_presentTargetHeight = 0;
	bool g_renderDocFound = false;

	void CheckForRenderDoc();
	void UpdateLazyKeypresses();

	// Core
	bool Init(WindowedMode windowedMode) {

		CheckForRenderDoc();
		Config::Init();

		if (RenderDocFound())
			std::cout << "RenderDoc found" << std::endl;

		if (!GLFWIntegration::Init(windowedMode)) {
			return false;
		}

		// OpenGL API
		OpenGLBackEnd::Init();
		OpenGLRenderer::Init();

		AssetManager::Init();
		UIBackEnd::Init();
		// Init sub-systems
		Input::Init(BackEnd::GetWindowPointer());
		Audio::Init();

		glfwShowWindow(static_cast<GLFWwindow*>(BackEnd::GetWindowPointer()));
		return true;
	}

	void UpdateGame() {

		const Resolutions& resolutions = Config::GetResolutions();
		float deltaTime;

		Game::Update();

		Debug::Update();
		UIBackEnd::Update();
		RenderDataManager::Update();
	}

	void BeginFrame() {
		GLFWIntegration::BeginFrame();
		RenderDataManager::BeginFrame();

		OpenGLBackEnd::BeginFrame();
		OpenGLBackEnd::UpdateTextureBaking();

		Game::BeginFrame();
	}

	void EndFrame() {
		GLFWIntegration::EndFrame();
		UIBackEnd::EndFrame();
		Debug::EndFrame();
	}

	void UpdateSubSystems() {
		Input::Update();
		Audio::Update();
		UpdateLazyKeypresses();
	}

	void CleanUp() {
		GLFWIntegration::Destroy();
	}

	// Window
	void BackEnd::ToggleFullscreen() {
		GLFWIntegration::ToggleFullscreen();
	}

	void BackEnd::ForceCloseWindow() {
		GLFWIntegration::ForceCloseWindow();
	}

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

	void CheckForRenderDoc() {
		#ifdef _WIN32
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
		if (snapshot == INVALID_HANDLE_VALUE) {
			g_renderDocFound = false;
		}

		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32);
		bool found = false;
		if (Module32First(snapshot, &moduleEntry)) {
			do {
				std::wstring wmodule(moduleEntry.szModule);
				std::string moduleName(wmodule.begin(), wmodule.end());

				if (moduleName.find("renderdoc.dll") != std::string::npos) {
					found = true;
					break;
				}
			} while (Module32Next(snapshot, &moduleEntry));
		}
		CloseHandle(snapshot);

		g_renderDocFound = found;
		#else
		g_renderDocActive = false;
		#endif
	}

	bool RenderDocFound() {
		return g_renderDocFound;
	}

	void UpdateLazyKeypresses() { 

		if (Input::KeyPressed(KEY_G)) {
			BackEnd::ToggleFullscreen();
		}

		if (Input::KeyPressed(KEY_ESCAPE)) {
			BackEnd::ForceCloseWindow();
		}

		if (Input::KeyPressed(KEY_GRAVE_ACCENT)) {
			Audio::PlayAudio(AUDIO_SELECT, 1.00f);
			Debug::ToggleDebugText();
		}
	}
}