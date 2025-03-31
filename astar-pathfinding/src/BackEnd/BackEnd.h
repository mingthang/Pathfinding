#pragma once

#include <Common/EngineEnums.h>

namespace BackEnd {
	// Core
	bool Init(WindowedMode windowedMode);
	void BeginFrame();
	void UpdateGame();
	void UpdateSubSystems();
	void EndFrame();
	void CleanUp();
    bool RenderDocFound();

	// Window
    void SetWindowedMode(const WindowedMode& windowedMode);
    const WindowedMode& GetWindowedMode();
    void* GetWindowPointer();
    void ToggleFullscreen();
    void ForceCloseWindow();
    bool WindowIsOpen();
    bool WindowHasFocus();
    bool WindowHasNotBeenForceClosed();
    bool WindowIsMinimized();
    int GetWindowedWidth();
    int GetWindowedHeight();
    int GetCurrentWindowWidth();
    int GetCurrentWindowHeight();
    int GetFullScreenWidth();
    int GetFullScreenHeight();
}
