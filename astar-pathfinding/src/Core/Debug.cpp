#include "Debug.h"

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
 }