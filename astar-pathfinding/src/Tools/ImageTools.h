#pragma once

#include <Common/EngineTypes.h>
#include <string>
#include <vector>

enum class CompressionType { DXT3, BC5, UNDEFINED };

namespace ImageTools {
	// Texture Data
	TextureData LoadUncompressedTextureData(const std::string& filePath);
}
