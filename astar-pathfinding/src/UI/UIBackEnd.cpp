#include "UIBackEnd.h"
#include "TextBlitter.h"

namespace UIBackEnd {
	Mesh2D g_uiMesh;
	std::vector<Vertex2D> g_vertices;
	std::vector<uint32_t> g_indices;
	std::vector<UIRenderItem> g_renderItems;

	void Init() {
		// Export fonts
		//std::string name = "RE_font";
		//std::string characters = R"(!"#$%&'*+,-./0123456789:;<=>?_ABCDEFGHIJKLMNOPQRSTUVWXYZ\^_`abcdefghijklmnopqrstuvwxyz )";
		//std::string textureSourcePath = "res/fonts/raw_images/standard_font/";
		//std::string outputPath = "res/fonts/";
		//FontSpriteSheetPacker::Export(name, characters, textureSourcePath, outputPath);

		// Import fonts
		FontSpriteSheet REFont = FontSpriteSheetPacker::Import("res/fonts/REFont.json");
		TextBlitter::AddFont(REFont);
	}

}