#include "AssetManager.h"
#include <API/OpenGL/GL_backEnd.h>
#include <Util/Util.h>
#include <future>
#include <iostream>

namespace AssetManager {
	void CompressMissingDDSTextures() {
		for (FileInfo& fileInfo : Util::IterateDirectory("res/textures/compress_me", { "png", "jpg", "tga" })) {
			std::string inputPath = fileInfo.path;
			std::string outputPath = "res/textures/compressed/" + fileInfo.name + ".dds";
			if (!Util::FileExists(outputPath)) {
				// ...
			}
		}
	}

	void LoadPeningTextureAsync() {
		std::vector<Texture>& textures = GetTextures();
		std::vector<std::future<void> > futures;

		for (Texture& texture : textures) {
			if (texture.GetLoadingState() == LoadingState::AWAITING_LOADING_FROM_DISK) {
				texture.SetLoadingState(LoadingState::LOADING_FROM_DISK);
				futures.emplace_back(std::async(std::launch::async, LoadTexture, &texture));
			}
		}

		for (auto& future : futures) {
			future.get();
		}

		// Allocate GPU memory
		for (Texture& texture : textures) {
			// OpenGL
			OpenGLBackEnd::AllocateTextureMemory(texture);
		}
	}

	void LoadTexture(Texture* texture) {
		if (texture) {
			texture->Load();
			// bake texture
			// ...
		}
	}


	// GET
	Texture* GetTextureByName(const std::string& name) {
		std::vector<Texture> textures = GetTextures();
		for (Texture& texture : textures) {
			if (texture.GetFileInfo().name == name)
				return &texture;
		}
		std::cout << "AssetManager::GetTextureByName(const std::string& name) failed because '" << name << "' does not exist\n";
		return nullptr;
	}

	Texture* GetTextureByIndex(int index) {
		std::vector<Texture> textures = GetTextures();
		if (index != -1)
			return &textures[index];
		std::cout << "AssetManager::GetTextureByIndex(int index) failed because '" << index << "' is not in range\n";
		return nullptr;
	}

	int GetTextureIndexByName(const std::string& name, bool ignoreWarning = true) {
		std::unordered_map<std::string, int>& indexMap = GetTextureIndexMap();
		auto it = indexMap.find(name);
		if (it != indexMap.end()) {
			return it->second;
		}
		if (!ignoreWarning) {
			std::cout << "AssetManager::GetTextureIndexByName(const std::string& name) failed because name '" << name << "' was not found in g_textures\n";
		}
		return -1;
	}

	int GetTextureCount() {
		return GetTextures().size();
	}
}