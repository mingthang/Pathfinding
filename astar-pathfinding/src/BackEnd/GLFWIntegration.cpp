#include "GLFWIntegration.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <unordered_map>

namespace GLFWIntegration {
	GLFWwindow* g_window = NULL;
	WindowedMode g_windowedMode = WindowedMode::WINDOWED;
	GLFWmonitor* g_monitor;
	const GLFWvidmode* g_mode;
	bool g_forceCloseWindow = false;
	int g_currentCursor;
	std::unordered_map<int, GLFWcursor*> g_cursorsPtrs;
	int g_fullscreenWidth = 0;
	int g_fullscreenHeight = 0;
	int g_windowedWidth = 0;
	int g_windowedHeight = 0;
	int g_currentWindowWidth = 0;
	int g_currentWindowHeight = 0;
	bool g_windowHasFocus = true;

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	// Core
	bool Init(WindowedMode windowedMode) {
		glfwInit();
		glfwSetErrorCallback([](int error_code, const char* description)
			{ std::cout << "GLFW Error (" << std::to_string(error_code) << "): " << description << std::endl; });

		// OPENGL API
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_DEBUG, GL_TRUE);
			
		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, GL_TRUE);

		// Resolution and window size
		g_monitor = glfwGetPrimaryMonitor();
		g_mode = glfwGetVideoMode(g_monitor);
		// Fullscreen Borderless
		glfwWindowHint(GLFW_RED_BITS, g_mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, g_mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, g_mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, g_mode->refreshRate);
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		g_fullscreenWidth = g_mode->width;
		g_fullscreenHeight = g_mode->height;
		g_windowedWidth = g_fullscreenWidth * 0.75f;
		g_windowedHeight = g_fullscreenHeight * 0.75f;

		g_windowedMode = windowedMode;
		if (windowedMode == WindowedMode::WINDOWED) {
			g_currentWindowWidth = g_windowedWidth;
			g_currentWindowHeight = g_windowedHeight;
			g_window = glfwCreateWindow(g_windowedWidth, g_windowedHeight, "Pathfinding", NULL, NULL);
			glfwSetWindowPos(g_window, 0, 0);
		}
		else if (windowedMode == WindowedMode::FULLSCREEN) {
			g_currentWindowWidth = g_fullscreenWidth;
			g_currentWindowHeight = g_fullscreenHeight;
			g_window = glfwCreateWindow(g_fullscreenWidth, g_fullscreenHeight, "Pathfinding", g_monitor, NULL);
		}

		if (g_window == NULL) {
			std::cout << "GLFWIntegration() failed to create window.\n";
			glfwTerminate();
			return false;
		}

		glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);

		g_cursorsPtrs[GLFW_ARROW_CURSOR] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		g_cursorsPtrs[GLFW_IBEAM_CURSOR] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		g_cursorsPtrs[GLFW_CROSSHAIR_CURSOR] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		g_cursorsPtrs[GLFW_HAND_CURSOR] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
		g_cursorsPtrs[GLFW_HRESIZE_CURSOR] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		g_cursorsPtrs[GLFW_VRESIZE_CURSOR] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

		return true;
	}

	void MakeContextCurrent() {
		glfwMakeContextCurrent(g_window);
	}

	void BeginFrame() {
		glfwPollEvents();

		int focused = glfwGetWindowAttrib(g_window, GLFW_FOCUSED);
		g_windowHasFocus = (focused != 0);
	}

	void EndFrame() {
		glfwSwapBuffers(g_window);
		return;
	}

	void Destroy() {
		if (GetWindowedMode() == WindowedMode::FULLSCREEN) {
			ToggleFullscreen();
		}
		glfwDestroyCursor(g_cursorsPtrs[GLFW_ARROW_CURSOR]);
		glfwDestroyCursor(g_cursorsPtrs[GLFW_IBEAM_CURSOR]);
		glfwDestroyCursor(g_cursorsPtrs[GLFW_CROSSHAIR_CURSOR]);
		glfwDestroyCursor(g_cursorsPtrs[GLFW_HAND_CURSOR]);
		glfwDestroyCursor(g_cursorsPtrs[GLFW_HRESIZE_CURSOR]);
		glfwDestroyCursor(g_cursorsPtrs[GLFW_VRESIZE_CURSOR]);
		glfwTerminate();
	}

	void SetWindowedMode(const WindowedMode& windowedMode) {
		if (windowedMode == WindowedMode::WINDOWED) {
			g_currentWindowWidth = g_windowedWidth;
			g_currentWindowHeight = g_windowedHeight;
			glfwSetWindowMonitor(g_window, nullptr, 0, 0, g_windowedWidth, g_windowedHeight, g_mode->refreshRate);
			glfwSetWindowPos(g_window, 0, 0);
		}
		else if (windowedMode == WindowedMode::FULLSCREEN) {
			g_currentWindowWidth = g_fullscreenWidth;
			g_currentWindowHeight = g_fullscreenHeight;
			glfwSetWindowMonitor(g_window, nullptr, 0, 0, g_fullscreenWidth - 1, g_fullscreenHeight - 1, g_mode->refreshRate);
			//glfwSetWindowMonitor(g_window, g_monitor, 0, 0, g_fullscreenWidth, g_fullscreenHeight, g_mode->refreshRate);
		}
		g_windowedMode = windowedMode;
	}

	void ToggleFullscreen() {
		if (g_windowedMode == WindowedMode::WINDOWED) {
			SetWindowedMode(WindowedMode::FULLSCREEN);
		}
		else {
			SetWindowedMode(WindowedMode::WINDOWED);
		}
	}

	void ForceCloseWindow() {
		g_forceCloseWindow = true;
	}

	void SetCursor(int cursor) {
		g_currentCursor = cursor;
		glfwSetCursor(g_window, g_cursorsPtrs[cursor]);
	}

	bool WindowHasFocus() {
		return g_windowHasFocus;
	}

	void* GetWindowPointer() {
		return g_window;
	}

	int GetWindowedWidth() {
		return g_windowedWidth;
	}

	int GetWindowedHeight() {
		return g_windowedHeight;
	}

	int GetFullScreenWidth() {
		return g_fullscreenWidth;
	}

	int GetFullScreenHeight() {
		return g_fullscreenHeight;
	}

	int GetCurrentWindowWidth() {
		return g_currentWindowWidth;
	}

	int GetCurrentWindowHeight() {
		return g_currentWindowHeight;
	}

	bool WindowIsOpen() {
		return !(glfwWindowShouldClose(g_window) || g_forceCloseWindow);
	}

	bool WindowIsMinimized() {
		int width = 0;
		int height = 0;
		glfwGetFramebufferSize(g_window, &width, &height);
		return (width == 0 || height == 0);
	}

	const WindowedMode& GetWindowedMode() {
		return g_windowedMode;
	}

	// Window handle
	//void* GetWin32Window() {
	//	return glfwGetWin32Window(g_window);
	//}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	}
}