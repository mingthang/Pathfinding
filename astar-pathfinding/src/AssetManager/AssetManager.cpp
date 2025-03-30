#include "AssetManager.h"
#include <UI/UIBackEnd.h>
#include <Util/Util.h>
#include <Common/EngineTypes.h>
#include <Renderer/Renderer.h>
#include <iostream>

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

        // Check texture bake

        // Load models

        g_loadingComplete = true;
        
        if (LoadingComplete()) {
        }

        // Free all cpu texture data
        for (Texture& texture : g_textures) {
            texture.FreeCPUMemory();
        }

        Renderer::InitMain();
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
}