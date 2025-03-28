#pragma once

#include "FontSpriteSheet.h"
#include <Common/EngineTypes.h>

namespace TextBlitter {
	void AddFont(const FontSpriteSheet& fontSpriteSheet);
	MeshData2D BlitText(
		const std::string& text, 
		const std::string& fontName, 
		int originX, 
		int originY, 
		glm::ivec2 viewportSize,
		Alignment alignment, 
		float scale, 
		uint32_t baseVertex);
	FontSpriteSheet* GetFontSpriteSheet(const std::string& name);
	glm::ivec2 GetBlitTextSize(const std::string& text, const std::string& fontName, float scale);
}
