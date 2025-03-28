#include "AssetManager.h"

namespace AssetManager {
	void CompressMissingDDSTextures() {
		for (FileInfo& fileInfo : Util::IterateDirectory("res/textures/compress_me", { "png", "jpg", "tga" })) {

		}
	}
}