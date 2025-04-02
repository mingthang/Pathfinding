#include "GridMap.h"

#include <glm/vec2.hpp>
#include <Config/Config.h>
#include <Core/Input/Input.h>
#include <Core/Pathfinding/Pathfinding.h>
#include <Core/Audio/Audio.h>
#include <BackEnd/BackEnd.h>
#include <Util/Util.h>
#include <API/OpenGL/Renderer/GL_renderer.h>

namespace GridMap {
	int g_mapWidth = 0;
	int g_mapHeight = 0;
	glm::ivec2 g_start;
	glm::ivec2 g_target;
	std::vector<std::vector<bool>> g_map;

	void Init() {
		const Resolutions& resolutions = Config::GetResolutions();

		g_mapWidth = resolutions.gBuffer.x / CELL_SIZE;
		g_mapHeight = resolutions.gBuffer.y / CELL_SIZE + 1;
		g_map.resize(g_mapWidth, std::vector<bool>(g_mapHeight, false));
	}

	void Update(float deltaTime) {
		// Add wall
		if (Input::LeftMouseDown()) {
			SetObstacle(GetMouseCellX(), GetMouseCellY(), true);
			Pathfinding::ResetAStar();
		}
		// Remove wall
		if (Input::RightMouseDown()) {
			SetObstacle(GetMouseCellX(), GetMouseCellY(), false);
			Pathfinding::ResetAStar();
		}
		// Clear Map
		if (Input::KeyPressed(KEY_N)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
			ClearMap();
			Pathfinding::ResetAStar();
		}
		// Toggle slow mode
		if (Input::KeyPressed(KEY_Q)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
			Pathfinding::ToggleSlowMode();
			Pathfinding::ResetAStar();
		}
		// Set start
		if (Input::KeyPressed(KEY_1)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
			SetStart(GetMouseCellX(), GetMouseCellY());
			Pathfinding::ResetAStar();
		}
		// Set end
		if (Input::KeyPressed(KEY_2)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
			SetTarget(GetMouseCellX(), GetMouseCellY());
			Pathfinding::ResetAStar();
		}
		// Find path
		if (Input::KeyPressed(KEY_SPACE)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
		}
		if (Input::KeyDown(KEY_SPACE) && !Pathfinding::GetAStar().GridPathFound()) {
			Audio::PlayAudio("UI_Select.wav", 0.5);
			if (!Pathfinding::GetAStar().SearchInitilized()) {
				Pathfinding::GetAStar().InitSearch(g_map, g_start.x, g_start.y, g_target.x, g_target.y);
			}
			if (!Pathfinding::GetAStar().GridPathFound()) {
				Pathfinding::GetAStar().FindPath();
			}
		}
		// Smooth path:
		if (Input::KeyPressed(KEY_W) || Input::KeyPressed(KEY_E)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
		}
		if (Input::KeyDown(KEY_W) && !Pathfinding::GetAStar().SmoothPathFound() || Input::KeyPressed(KEY_E)) {
			Audio::PlayAudio("UI_Select.wav", 0.5);
			if (!Pathfinding::GetAStar().SmoothPathFound()) {
				Pathfinding::GetAStar().FindSmoothPath();
			}
		}
	}

	void ClearMap() {
		for (int y = 0; y < GetMapWidth(); ++y) {
			for (int x = 0; x < GetMapHeight(); ++x) {
				g_map[y][x] = false;
			}
		}
		g_start = { 0,0 };
		g_target = { 0,1 };
	}

	void SetStart(int x, int y) {
		if (IsInBounds(x, y)) {
			g_start = { x , y };
		}
	}

	void SetTarget(int x, int y) {
		if (IsInBounds(x, y)) {
			g_target = { x , y };
		}
	}

	bool IsInBounds(int x, int y) {
		return (x >= 0 && y >= 0 && x < g_mapWidth && y < g_mapHeight);
	}

	void SetObstacle(int x, int y, bool value) {
		if (IsInBounds(x, y)) {
			g_map[x][y] = value;
		}
	}

	bool IsObstacle(int x, int y) {
		if (IsInBounds(x, y)) {
			return g_map[x][y];
		}
		else {
			return false;
		}
	}

	std::vector<RenderItem2D> CreateRenderItems2D() {
		const Resolutions& resolutions = Config::GetResolutions();
		glm::ivec2 location = glm::ivec2(0.0f, (float)resolutions.gBuffer.y);
		glm::ivec2 viewportSize = glm::ivec2(resolutions.gBuffer.x, resolutions.gBuffer.y);
		std::vector<RenderItem2D> renderItems;

		int drawX = Util::MapRange(Input::GetMouseX(), 0, BackEnd::GetCurrentWindowWidth(), 0, resolutions.gBuffer.x);
		int drawY = Util::MapRange(Input::GetMouseY(), 0, BackEnd::GetCurrentWindowHeight(), 0, resolutions.gBuffer.y);
		drawX /= CELL_SIZE;
		drawY /= CELL_SIZE;
		drawX *= CELL_SIZE;
		drawY *= CELL_SIZE;
		drawY = resolutions.gBuffer.y - drawY;

		int cellX = Util::MapRange(Input::GetMouseX(), 0, BackEnd::GetCurrentWindowWidth(), 0, resolutions.gBuffer.x) / CELL_SIZE;
		int cellY = Util::MapRange(Input::GetMouseY(), 0, BackEnd::GetCurrentWindowHeight(), 0, resolutions.gBuffer.y) / CELL_SIZE;

		std::string text = "";
		text += "Cell X: " + std::to_string(GetMouseCellX()) + "\n";
		text += "Cell Y: " + std::to_string(GetMouseCellY()) + "\n";
		text += "Mouse X: " + std::to_string(GetMouseX()) + "\n";
		text += "Mouse Y: " + std::to_string(GetMouseY()) + "\n";
		if (Pathfinding::SlowModeEnabled()) {
			text += "Slowmode: On\n";
		}
		else {
			text += "Slowmode: Off\n";
		}

		for (int x = 0; x < GetMapWidth(); x++) {
			for (int y = 0; y < GetMapHeight(); y++) {
				glm::ivec2 drawLocation = glm::ivec2(x * CELL_SIZE, resolutions.gBuffer.y - y * CELL_SIZE);

				if (IsObstacle(x, y)) {
					renderItems.push_back(OpenGLRenderer::CreateRenderItem2D("tile_wall", drawLocation, viewportSize, Alignment::TOP_LEFT));
				}
				else {
					renderItems.push_back(OpenGLRenderer::CreateRenderItem2D("tile", drawLocation, viewportSize, Alignment::TOP_LEFT));
				}
			}
		}

		AStar aStar = Pathfinding::GetAStar();

		for (auto& cell : aStar.GetClosedList()) {
			renderItems.push_back(CreateColoredTile(cell->x, cell->y, glm::vec3(1, 0, 0))); // RED
		}

		for (int i = 0; i < aStar.GetOpenList().Size(); i++) {
			auto& cell = aStar.GetOpenList().items[i];
			renderItems.push_back(CreateColoredTile(cell->x, cell->y, glm::vec3(0, 1, 0))); // GREEN
		}
		for (auto& cell : aStar.GetPath()) {
			renderItems.push_back(CreateColoredTile(cell->x, cell->y, glm::vec3(0, 0, 1))); // BLUE
		}

		renderItems.push_back(CreateColoredTile(GetStartX(), GetStartY(), glm::vec3(0.164f, 0.605f, 0.765f)));
		renderItems.push_back(CreateColoredTile(GetTargetX(), GetTargetY(), glm::vec3(0.164f, 0.605f, 0.765f)));

		glm::ivec2 drawLocation = glm::ivec2(drawX, drawY);
		renderItems.push_back(OpenGLRenderer::CreateRenderItem2D("selector", drawLocation, viewportSize, Alignment::TOP_LEFT));
		//OpenGLRenderer::AddRenderItems(renderItems, TextBlitter::BlitText(text, location, viewportSize, Alignment::TOP_LEFT, BitmapFontType::STANDARD));

		return renderItems;

	}

	RenderItem2D CreateColoredTile(int x, int y, glm::vec3 color) {
		const Resolutions& resolutions = Config::GetResolutions();
		glm::ivec2 viewportSize = glm::ivec2(resolutions.gBuffer.x, resolutions.gBuffer.y);
		return OpenGLRenderer::CreateRenderItem2D("tile", { x * CELL_SIZE, resolutions.gBuffer.y - y * CELL_SIZE }, viewportSize, Alignment::TOP_LEFT, color);
	}

	int GetMouseX() {
		const Resolutions& resolutions = Config::GetResolutions();
		float scalingRatio = BackEnd::GetCurrentWindowWidth() / resolutions.gBuffer.x;
		return Util::MapRange(Input::GetMouseX(), 0, BackEnd::GetCurrentWindowWidth(), 0, resolutions.gBuffer.x);
	}

	int GetMouseY() {
		const Resolutions& resolutions = Config::GetResolutions();
		return Util::MapRange(Input::GetMouseY(), 0, BackEnd::GetCurrentWindowHeight(), 0, resolutions.gBuffer.y);
	}

	int GetMouseCellX() {
		const Resolutions& resolutions = Config::GetResolutions();
		return Util::MapRange(Input::GetMouseX(), 0, BackEnd::GetCurrentWindowWidth(), 0, resolutions.gBuffer.x) / CELL_SIZE;
	}

	int GetMouseCellY() {
		const Resolutions& resolutions = Config::GetResolutions();
		return Util::MapRange(Input::GetMouseY(), 0, BackEnd::GetCurrentWindowHeight(), 0, resolutions.gBuffer.y) / CELL_SIZE;
	}

	int GetMapWidth() {
		return g_mapWidth;
	}

	int GetMapHeight() {
		return g_mapHeight;
	}

	int GetStartX() {
		return g_start.x;
	}

	int GetStartY() {
		return g_start.y;
	}

	int GetTargetX() {
		return g_target.x;
	}

	int GetTargetY() {
		return g_target.y;
	}


}

float Cell::GetF(Cell* destination) {
	if (f == -1) {
		f = g + GetH(destination);
	}
	return f;
}

float Cell::GetH(Cell* destination) {
	if (h == -1) {
		int dstX = std::abs(x - destination->x);
		int dstY = std::abs(y - destination->y);
		if (dstX > dstY) {
			h = DIAGONAL_COST * dstY + ORTHOGONAL_COST * (dstX - dstY);
		}
		else {
			h = DIAGONAL_COST * dstX + ORTHOGONAL_COST * (dstY - dstX);
		}
	}
	return h;
}