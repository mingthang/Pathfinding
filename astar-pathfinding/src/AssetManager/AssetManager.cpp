#include "AssetManager.h"

namespace AssetManager {

    std::vector<std::string> g_loadLog;
    bool g_loadingComplete = false;

    void Init() {

    }
    void UpdateLoading() {
        // Calculate load log text
        std::string text = "";
        int maxLinesDisplayed = 36;
        int endIndex = AssetManager::GetLoadLog().size();
        int beginIndex = std::max(0, endIndex - maxLinesDisplayed);
        for (int i = beginIndex; i < endIndex; i++) {
            text += AssetManager::GetLoadLog()[i] + "\n";
        }

    }

    bool LoadingComplete() {
        return g_loadingComplete;
    }

    std::vector<std::string>& GetLoadLog() {
        return g_loadLog;
    }
}