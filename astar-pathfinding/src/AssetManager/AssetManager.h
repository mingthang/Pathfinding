#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <Renderer/Types/Texture.h>
#include <Renderer/Types/Mesh.h>
#include <Renderer/Types/Model.h>

namespace AssetManager {
	void Init();
	void UpdateLoading();
	bool LoadingComplete();
	void AddItemToLoadLog(std::string text);
	std::vector<std::string>& GetLoadLog();

	// Loading
	void LoadTexture(Texture* texture);
	//void LoadModel(Model* model);
	void LoadPendingTexturesAsync();
	//void LoadPendingModelsAsync();

	// Building
	void BuildTextureIndexMap();

	// Baking

	// Vertex Data
	std::vector<Vertex>& GetVertices();
	std::vector<uint32_t>& GetIndices();

	// Mesh
	std::vector<Mesh>& GetMeshes();
	int CreateMesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, glm::vec3 aabbMin = glm::vec3(0), glm::vec3 aabbMax = glm::vec3(0));
	Mesh* GetMeshByIndex(int index);
	int GetMeshIndexByName(const std::string& name);
	Mesh* GetMeshByName(const std::string& name);
	std::vector<Vertex> GetMeshVertices(Mesh* mesh);

	// Models
	Model* GetModelByIndex(int index);
	int GetModelIndexByName(const std::string& name);
	bool ModelExists(const std::string& name);
	void CreateHardcodedModels();
	unsigned int GetQuadMeshIndex();

	// Textures
	std::vector<Texture>& GetTextures();
	Texture* GetTextureByName(const std::string& name);
	Texture* GetTextureByIndex(int index);
	int GetTextureIndexByName(const std::string& name, bool ignoreWarning = false);
	int GetTextureCount();

	// Index maps
	std::unordered_map<std::string, int>& GetTextureIndexMap();
}
