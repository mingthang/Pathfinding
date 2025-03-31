#pragma once
#include <Common/EngineEnums.h>

namespace GLFWIntegration {
	// Core
	bool Init(WindowedMode windowedMode);
    void Destroy();
    void BeginFrame();
    void EndFrame();
    void SetWindowedMode(const WindowedMode& windowedMode);
    void ToggleFullscreen();
    void ForceCloseWindow();
    bool WindowIsOpen();
    bool WindowHasFocus();
    bool WindowIsMinimized();
    int GetWindowedWidth();
    int GetWindowedHeight();
    int GetCurrentWindowWidth();
    int GetCurrentWindowHeight();
    int GetFullScreenWidth();
    int GetFullScreenHeight();
    void* GetWindowPointer();
    const WindowedMode& GetWindowedMode();
    void SetCursor(int);
    
    // Windows only
    //void* GetWin32Window();

    // OpenGL only
    void MakeContextCurrent();
};
