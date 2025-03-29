#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <Renderer/Types/Texture.h>

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

	// Textures
	std::vector<Texture>& GetTextures();
	Texture* GetTextureByName(const std::string& name);
	Texture* GetTextureByIndex(int index);
	int GetTextureIndexByName(const std::string& name, bool ignoreWarning = true);
	int GetTextureCount();

	// Index maps
	std::unordered_map<std::string, int>& GetTextureIndexMap();
}
