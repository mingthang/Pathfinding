#include "TextBlitter.h"
#include <unordered_map>
#include <iostream>

namespace TextBlitter {

	std::vector<FontSpriteSheet> g_fontSpriteSheets;
	std::unordered_map<std::string, uint32_t> g_fontIndices;

	void AddFont(const FontSpriteSheet& fontSpriteSheet) {
		if (g_fontIndices.find(fontSpriteSheet.m_name) != g_fontIndices.end()) {
			std::cout << "Font already exists: " << fontSpriteSheet.m_name << "\n";
			return;
		}
		uint32_t index = g_fontSpriteSheets.size();
		g_fontIndices[fontSpriteSheet.m_name] = index;
		g_fontSpriteSheets.push_back(fontSpriteSheet);
	}

	MeshData2D BlitText(const std::string& text, const std::string& fontName, int originX, int originY, glm::ivec2 viewportSize, Alignment alignment, float scale, uint32_t baseVertex) {
		FontSpriteSheet* spriteSheet = GetFontSpriteSheet(fontName);
		MeshData2D meshData;
		int textureIndex = AssetManager::GetTextureIndexByName(fontName);

		// Bail if spritesheet or font texture not found
		if (!spriteSheet || textureIndex == -1) {
			return meshData;
		}
	}

	FontSpriteSheet* GetFontSpriteSheet(const std::string& name) {
		auto it = g_fontIndices.find(name);
		return (it != g_fontIndices.end()) ? &g_fontSpriteSheets[it->second] : nullptr;
	}

}