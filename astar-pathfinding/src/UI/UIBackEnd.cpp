#include "UIBackEnd.h"
#include "TextBlitter.h"
#include <AssetManager/AssetManager.h>
#include <Config/Config.h>
#include <iostream>

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

	void Update() {

		//if (Debug::IsDebugTextVisible()) {
		//	BlitText(Debug::GetText(), "REFont", 0, 0, Alignment::TOP_LEFT, 2.0f);
		//}

		const Resolutions& resolutions = Config::GetResolutions();

		// OPENGL API
		g_uiMesh.GetGLMesh2D().UpdateVertexBuffer(g_vertices, g_indices);

		g_vertices.clear();
		g_indices.clear();
	}

	void BlitText(const std::string& text, const std::string& fontName, int originX, int originY, Alignment alignment, float scale, TextureFilter textureFilter = TextureFilter::NEAREST) {
		// Check if font spritesheet avaliable
		FontSpriteSheet* fontSpriteSheet = TextBlitter::GetFontSpriteSheet(fontName);
		if (!fontSpriteSheet) {
			std::cout << "UIBackEnd::BlitText() failed to find " << fontName << "\n";
			return;
		}
		int baseVertex = g_vertices.size();

		const Resolutions& resolutions = Config::GetResolutions();

		MeshData2D meshData = TextBlitter::BlitText(text, fontName, originX, originY, resolutions.ui, alignment, scale, baseVertex);
		g_vertices.insert(std::end(g_vertices), std::begin(meshData.vertices), std::end(meshData.vertices));
		g_indices.insert(std::end(g_indices), std::begin(meshData.indices), std::end(meshData.indices));

		UIRenderItem& renderItem = g_renderItems.emplace_back();
		renderItem.baseVertex = 0;
		renderItem.baseIndex = g_indices.size() - meshData.indices.size();
		renderItem.indexCount = meshData.indices.size();
		renderItem.textureIndex = AssetManager::GetTextureIndexByName(fontName);
		renderItem.filter = (textureFilter == TextureFilter::NEAREST) ? 1 : 0;
	}

	void EndFrame() {
		g_renderItems.clear();
	}

	Mesh2D& GetUIMesh() {
		return g_uiMesh;
	}

	std::vector<UIRenderItem>& GetRenderItems() {
		return g_renderItems;
	}
}