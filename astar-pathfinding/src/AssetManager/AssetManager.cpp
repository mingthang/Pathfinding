#include "AssetManager.h"
#include "BakeQueue.h"
#include <UI/UIBackEnd.h>
#include <Util/Util.h>
#include <Common/EngineTypes.h>
#include <Renderer/Renderer.h>
#include <iostream>
#include <mutex>

namespace AssetManager {

    std::vector<Texture> g_textures;

    std::unordered_map<std::string, int> g_textureIndexMap;

    std::vector<std::string> g_loadLog;
    bool g_loadingComplete = false;

    void FindAssetPaths();
    void LoadFontTextures();

    void Init() {
        LoadFontTextures();
        FindAssetPaths();

        LoadPendingTexturesAsync();
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

        UIBackEnd::BlitText(text, "REFont", 0, 0, Alignment::TOP_LEFT, 2.0f);

        // Loading complete?
        g_loadingComplete = true;
        // Check texture bake
        // TODO: UpdateTextureBaking 
        for (Texture& texture : g_textures) {
            texture.CheckForBakeCompletion();
            if (!texture.BakeComplete()) {
                g_loadingComplete = false;
                return;
            }
        }
        // Load models
        
        if (LoadingComplete()) {
        }

        // Free all CPU texture data
        for (Texture& texture : g_textures) {
            texture.FreeCPUMemory();
        }

        //Renderer::InitMain();
        std::cout << "Assets loaded\n";
    }

    bool LoadingComplete() {
        return g_loadingComplete;
    }

    void FindAssetPaths() {
        // Find all textures
        for (FileInfo& fileInfo : Util::IterateDirectory("res/textures/uncompressed", { "png", "jpg", "tga" })) {
            Texture& texture = g_textures.emplace_back();
            texture.SetFileInfo(fileInfo);
            texture.SetImageDataType(ImageDataType::UNCOMPRESSED);
            texture.SetTextureWrapMode(TextureWrapMode::REPEAT);
            texture.SetMinFilter(TextureFilter::LINEAR_MIPMAP);
            texture.SetMagFilter(TextureFilter::LINEAR);
            texture.RequestMipmaps();
        }
    }

    void AddItemToLoadLog(std::string text) {
        std::replace(text.begin(), text.end(), '\\', '/');
        g_loadLog.push_back(text);
        if (false) {
            static std::mutex mutex;
            std::lock_guard<std::mutex> lock(mutex);
            std::cout << text << "\n";
        }
    }

    // Loading
    void LoadFontTextures() {
        for (FileInfo& fileInfo : Util::IterateDirectory("res/fonts", { "png" })) {
            Texture& texture = g_textures.emplace_back();
            texture.SetFileInfo(fileInfo);
            texture.SetImageDataType(ImageDataType::UNCOMPRESSED);
            texture.SetTextureWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
            texture.SetMinFilter(TextureFilter::NEAREST);
            texture.SetMagFilter(TextureFilter::NEAREST);
        }
        LoadPendingTexturesAsync();
        BakeQueue::ImmediateBakeAllTextures();
        BuildTextureIndexMap();
    }

    void BuildTextureIndexMap() {
        g_textureIndexMap.clear();
        for (int i = 0; i < g_textures.size(); i++) {
            g_textureIndexMap[g_textures[i].GetFileInfo().name] = i;
        }
    }

    std::vector<std::string>& GetLoadLog() {
        return g_loadLog;
    }

    // GET
    std::vector<Texture>& GetTextures() {
        return g_textures;
    }

    std::unordered_map<std::string, int>& GetTextureIndexMap() {
        return g_textureIndexMap;
    }
}