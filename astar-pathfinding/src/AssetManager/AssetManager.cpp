#include "AssetManager.h"
#include "BakeQueue.h"
#include <UI/UIBackEnd.h>
#include <Util/Util.h>
#include <Common/EngineTypes.h>
#include <Renderer/Renderer.h>
#include <iostream>
#include <mutex>
#include <API/OpenGL/GL_backEnd.h>

namespace AssetManager {

    std::vector<Mesh> g_meshes;
    std::vector<Model> g_models;
    std::vector<Texture> g_textures;

    std::unordered_map<std::string, int> g_textureIndexMap;

    std::vector<Vertex> g_vertices;
    std::vector<uint32_t> g_indices;

    std::vector<std::string> g_loadLog;
    bool g_loadingComplete = false;

    void FindAssetPaths();
    void LoadFontTextures();

    void Init() {
        CreateHardcodedModels();
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
        for (Texture& texture : g_textures) {
            texture.CheckForBakeCompletion();
            if (!texture.BakeComplete()) {
                g_loadingComplete = false;
                return;
            }
        }

        // Load models
        if (LoadingComplete()) {
            BuildTextureIndexMap();
            OpenGLBackEnd::UploadVertexData(g_vertices, g_indices);
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
            //texture.RequestMipmaps();
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
    std::vector<Vertex>& GetVertices() {
        return g_vertices;
    }

    std::vector<uint32_t>& GetIndices() {
        return g_indices;
    }

    std::vector<Mesh>& GetMeshes() {
        return g_meshes;
    }

    std::vector<Texture>& GetTextures() {
        return g_textures;
    }

    std::unordered_map<std::string, int>& GetTextureIndexMap() {
        return g_textureIndexMap;
    }

    void CreateHardcodedModels() {
        /* Quad */ {
            Vertex vertA, vertB, vertC, vertD;
            vertA.position = { -1.0f, -1.0f, 0.0f };
            vertB.position = { -1.0f, 1.0f, 0.0f };
            vertC.position = { 1.0f,  1.0f, 0.0f };
            vertD.position = { 1.0f,  -1.0f, 0.0f };
            vertA.uv = { 0.0f, 0.0f };
            vertB.uv = { 0.0f, 1.0f };
            vertC.uv = { 1.0f, 1.0f };
            vertD.uv = { 1.0f, 0.0f };
            vertA.normal = glm::vec3(0, 0, 1);
            vertB.normal = glm::vec3(0, 0, 1);
            vertC.normal = glm::vec3(0, 0, 1);
            vertD.normal = glm::vec3(0, 0, 1);
            vertA.tangent = glm::vec3(1, 0, 0);
            vertB.tangent = glm::vec3(1, 0, 0);
            vertC.tangent = glm::vec3(1, 0, 0);
            vertD.tangent = glm::vec3(1, 0, 0);
            std::vector<Vertex> vertices;
            vertices.push_back(vertA);
            vertices.push_back(vertB);
            vertices.push_back(vertC);
            vertices.push_back(vertD);
            std::vector<uint32_t> indices = { 2, 1, 0, 3, 2, 0 };
            std::string name = "Quad";

            Model& model = g_models.emplace_back();
            model.SetName(name);
            model.AddMeshIndex(AssetManager::CreateMesh("Quad", vertices, indices));
        }

    }
}