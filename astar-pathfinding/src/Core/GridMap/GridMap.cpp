#include "GridMap.h"

#include <glm/vec2.hpp>
#include <Config/Config.h>
#include <Core/Input/Input.h>
#include <Core/Pathfinding/Pathfinding.h>
#include <Core/Audio/Audio.h>
#include <BackEnd/BackEnd.h>
#include <Util/Util.h>
#include <API/OpenGL/Renderer/GL_renderer.h>
#include <Core/Debug.h>
#include <UI/UIBackEnd.h>

namespace GridMap {
	int g_mapWidth = 0;
	int g_mapHeight = 0;
	glm::ivec2 g_start;
	glm::ivec2 g_target;
	std::vector<std::vector<bool>> g_map;

	std::vector<RenderItem2D> gridMap;

	//std::vector<RenderItem2D> pathLayer; // A* (open, closed, path)
	//std::vector<RenderItem2D> selectorLayer;

	bool bShouldUpdateGrid = false;
	bool bShouldPartialUpdate = false; // For partial updates of the grid layer
	bool bShouldUpdatePath = false;
	bool bFindPath = false;
	bool bRemoveWall = false;

	void Init() {
		const Resolutions& resolutions = Config::GetResolutions();

		g_mapWidth = resolutions.gBuffer.x / CELL_SIZE;
		g_mapHeight = resolutions.gBuffer.y / CELL_SIZE;
		g_map.resize(g_mapWidth, std::vector<bool>(g_mapHeight, false));

		// Init grid layer
		UpdateGridLayer();
		OpenGLRenderer::InitInstanceGridBuffers();
	}

	void Update(float deltaTime) {
		// Add wall
		if (Input::LeftMouseDown()) {
			SetObstacle(GetMouseCellX(), GetMouseCellY(), true);
			Pathfinding::ResetAStar();
			if(!bFindPath)
				bShouldPartialUpdate = true;
			else {
				bShouldUpdateGrid = true;
			}
		}
		// Remove wall
		if (Input::RightMouseDown()) {
			SetObstacle(GetMouseCellX(), GetMouseCellY(), false);
			Pathfinding::ResetAStar();
			if(!bFindPath)
				bShouldPartialUpdate = true;
			else {
				bShouldUpdateGrid = true;
			}
			bRemoveWall = true;
		}
		// Clear Map
		if (Input::KeyPressed(KEY_N)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
			ClearMap();
			Pathfinding::ResetAStar();
			bShouldUpdateGrid = true;
			bShouldUpdatePath = true;
		}
		// Toggle slow mode
		if (Input::KeyPressed(KEY_Q)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
			Pathfinding::ToggleSlowMode();
			Pathfinding::ResetAStar();
			bShouldUpdateGrid = true;
			bShouldUpdatePath = true;
		}
		// Set start
		if (Input::KeyPressed(KEY_1)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
			SetStart(GetMouseCellX(), GetMouseCellY());
			Pathfinding::ResetAStar();
			bShouldUpdateGrid = true;
			bShouldUpdatePath = true;
		}
		// Set end
		if (Input::KeyPressed(KEY_2)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
			SetTarget(GetMouseCellX(), GetMouseCellY());
			Pathfinding::ResetAStar();
			bShouldUpdateGrid = true;
			bShouldUpdatePath = true;
		}
		// Find path
		if (Input::KeyPressed(KEY_SPACE)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
			bShouldUpdateGrid = true;
			bShouldUpdatePath = true;
		}
		if (Input::KeyDown(KEY_SPACE) && !Pathfinding::GetAStar().GridPathFound()) {
			Audio::PlayAudio("UI_Select.wav", 0.5);
			if (!Pathfinding::GetAStar().SearchInitilized()) {
				Pathfinding::GetAStar().InitSearch(g_map, g_start.x, g_start.y, g_target.x, g_target.y);
			}
			if (!Pathfinding::GetAStar().GridPathFound()) {
				Pathfinding::GetAStar().FindPath();
			}
			bShouldUpdatePath = true;
		}
		// Smooth path:
		if (Input::KeyPressed(KEY_W) || Input::KeyPressed(KEY_E)) {
			Audio::PlayAudio("SELECT.wav", 1.0);
		}
		if ((Input::KeyDown(KEY_W) || Input::KeyPressed(KEY_E)) && !Pathfinding::GetAStar().SmoothPathFound()) {
			Audio::PlayAudio("UI_Select.wav", 0.5);
			bool useSlowMode = Input::KeyDown(KEY_W); // W slow, E fast 
			bool slowModeEnabled = Pathfinding::SlowModeEnabled();

			if (useSlowMode && !slowModeEnabled) Pathfinding::ToggleSlowMode();
			if (!useSlowMode && slowModeEnabled) Pathfinding::ToggleSlowMode();

			Pathfinding::GetAStar().FindSmoothPath();

			if (useSlowMode && !slowModeEnabled) Pathfinding::ToggleSlowMode();
			if (!useSlowMode && slowModeEnabled) Pathfinding::ToggleSlowMode();

			bShouldUpdatePath = true;
		}


		UpdateGridRenderItems();
		UpdateDebugGridText();
	}

	void ClearMap() {
		//for (int y = 0; y < GetMapWidth(); ++y) {
		//	for (int x = 0; x < GetMapHeight(); ++x) {
		//		g_map[y][x] = false;
		//	}
		//}
		for (int x = 0; x < GetMapWidth(); ++x) {
			for (int y = 0; y < GetMapHeight(); ++y) {
				g_map[x][y] = false;
			}
		}
		g_start = { 0, 0 };
		g_target = { 0, 1 };
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

	void UpdateDebugGridText() {
		int drawX = (GetMouseX() / CELL_SIZE) * CELL_SIZE;
		int drawY = (GetMouseY() / CELL_SIZE) * CELL_SIZE;
		drawX /= CELL_SIZE;
		drawY /= CELL_SIZE;
		drawX *= CELL_SIZE;
		drawY *= CELL_SIZE;

		int cellX = GetMouseX() / CELL_SIZE;
		int cellY = GetMouseY() / CELL_SIZE;

		std::string text = "\n";
		text += "Cell X: " + std::to_string(GetMouseCellX()) + "\n";
		text += "Cell Y: " + std::to_string(GetMouseCellY()) + "\n";
		text += "Grid Mouse X: " + std::to_string(GetMouseX()) + "\n";
		text += "Grid Mouse Y: " + std::to_string(GetMouseY()) + "\n";
		if (Pathfinding::SlowModeEnabled()) {
			text += "Slowmode: On\n";
		}
		else {
			text += "Slowmode: Off\n";
		}

		Debug::AddText(text);
	}

	void UpdateGridLayer() {
		gridMap.clear();
		const Resolutions& resolutions = Config::GetResolutions();
		glm::ivec2 viewportSize = glm::ivec2(resolutions.gBuffer.x, resolutions.gBuffer.y);

		for (int x = 0; x < GetMapWidth(); x++) {
			for (int y = 0; y < GetMapHeight(); y++) {
				if (IsObstacle(x, y)) {
					gridMap.push_back(CreateColoredTile(x, y, glm::vec3(0.4f, 0.4f, 0.4f)));
				}
				else {
					gridMap.push_back(CreateColoredTile(x, y, glm::vec3(1.0f, 1.0f, 1.0f)));
				}
			}
		}
	}

	void PartialUpdateGridLayer() {
		const Resolutions& resolutions = Config::GetResolutions();
		glm::ivec2 viewportSize = glm::ivec2(resolutions.gBuffer.x, resolutions.gBuffer.y);

		for (int x = 0; x < GetMapWidth(); x++) {
			for (int y = 0; y < GetMapHeight(); y++) {
				int index = x * GetMapHeight() + y;
				if (IsObstacle(x, y)) {
					gridMap[index] = CreateColoredTile(x, y, glm::vec3(0.4f, 0.4f, 0.4f));
				}
				if ((x == GetMouseCellX() && y == GetMouseCellY()) && bRemoveWall) {
					gridMap[index] = CreateColoredTile(x, y, glm::vec3(1.0f, 1.0f, 1.0f));
					bRemoveWall = false;
				}
			}
		}
	}

	void UpdatePathLayer() {
		const Resolutions& resolutions = Config::GetResolutions();
		glm::ivec2 viewportSize = glm::ivec2(resolutions.gBuffer.x, resolutions.gBuffer.y);

		AStar aStar = Pathfinding::GetAStar();
		// Closed list
		for (auto& cell : aStar.GetClosedList()) {
			int index = cell->x * GetMapHeight() + cell->y;
			gridMap[index] = CreateColoredTile(cell->x, cell->y, glm::vec3(0.9f, 0, 0));
		}
		// Open list
		for (int i = 0; i < aStar.GetOpenList().Size(); i++) {
			auto& cell = aStar.GetOpenList().items[i];
			int index = cell->x * GetMapHeight() + cell->y;
			gridMap[index] = CreateColoredTile(cell->x, cell->y, glm::vec3(0, 0.9f, 0));
		}
		// Path
		for (auto& cell : aStar.GetPath()) {
			int index = cell->x * GetMapHeight() + cell->y;
			gridMap[index] = CreateColoredTile(cell->x, cell->y, glm::vec3(0, 0, 0.9f));
		}

		bFindPath = true;
	}

	RenderItem2D CreateColoredTile(int x, int y, glm::vec3 color) {
		const Resolutions& resolutions = Config::GetResolutions();
		glm::ivec2 viewportSize = glm::ivec2(resolutions.gBuffer.x, resolutions.gBuffer.y);
		return OpenGLRenderer::CreateRenderItem2D("tile", { x * CELL_SIZE, y * CELL_SIZE }, viewportSize, Alignment::TOP_LEFT, color);
	}

	void UpdateGridRenderItems() {
		if (bShouldUpdateGrid) {
			UpdateGridLayer();
			bShouldUpdateGrid = false;
		}
		else {
			PartialUpdateGridLayer();
		}
		if (bShouldUpdatePath) {
			UpdatePathLayer();
			bShouldUpdatePath = false;
		}
	}

	int GetMouseX() {
		const Resolutions& resolutions = Config::GetResolutions();
		//float scalingRatio = BackEnd::GetCurrentWindowWidth() / resolutions.gBuffer.x;
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

	const std::vector<RenderItem2D>& GetGridLayer() {
		return gridMap;
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