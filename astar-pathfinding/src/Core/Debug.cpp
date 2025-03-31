#include "Debug.h"
#include <BackEnd/BackEnd.h>
#include <Core/Input/Input.h>
#include <Config/Config.h>
#include <iostream>

namespace Debug {
    std::string g_text = "";
    bool g_showDebugText = false;
    DebugRenderMode g_debugRenderMode = DebugRenderMode::NONE;

    void UpdateDebugPointsAndLines();
    void UpdateDebugText();

    void Update() {
        UpdateDebugPointsAndLines();
        UpdateDebugText();
    }

    void UpdateDebugText() {
        if (!g_showDebugText) return;

        const Resolutions& resolutions = Config::GetResolutions();

        int mouseX = Input::GetMouseX();
        int mouseY = Input::GetMouseY();
        int windowWidth = BackEnd::GetCurrentWindowWidth();
        int windowHeight = BackEnd::GetCurrentWindowHeight();
        int gBufferWidth = resolutions.gBuffer.x;
        int gBufferHeight = resolutions.gBuffer.y;

        AddText("");
        AddText("WindowWidth: " + std::to_string(windowWidth));
        AddText("WindowHeight: " + std::to_string(windowHeight));
        AddText("gBufferWidth: " + std::to_string(gBufferWidth));
        AddText("gBufferHeight: " + std::to_string(gBufferHeight));
        AddText("mouseX: " + std::to_string(mouseX));
        AddText("mouseY: " + std::to_string(mouseY));
    }

    void UpdateDebugPointsAndLines() {
        // Here check render mode: bones,  decals, ...
        return;
    }

    void EndFrame() {
        g_text = "";
    }

    void AddText(const std::string& text) {
        g_text += text + "\n";
    }

    const std::string& GetText() {
        return g_text;
    }

    void ToggleDebugText() {
        g_showDebugText = !g_showDebugText;
    }

    bool IsDebugTextVisible() {
        return g_showDebugText;
    }
 }