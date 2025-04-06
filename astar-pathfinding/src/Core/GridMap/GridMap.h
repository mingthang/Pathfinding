#pragma once

#include <vector>
#include <Common/EngineTypes.h>

#define CELL_SIZE 30
#define ORTHOGONAL_COST 30
#define DIAGONAL_COST 42

struct Cell {
	int x, y;
	bool obstacle;
	int g = 99999;  // G cost: distance from starting node
	int h = -1;     // H cost: distance from end node. Aka the heuristic.
	int f = -1;     // F cost: g + f
	std::vector<Cell*> neighbours;
	Cell* parent = nullptr;
	int heapIndex = -1;
	float GetF(Cell* destination);
	float GetH(Cell* destination);
};

namespace GridMap {
	void Init();
	void Update(float deltaTime);
	void ClearMap();
	void SetStart(int x, int y);;
	void SetTarget(int x, int y);
	void SetObstacle(int x, int y, bool value);

	bool IsInBounds(int x, int y);
	bool IsObstacle(int x, int y);

	RenderItem2D CreateColoredTile(int x, int y, glm::vec3 color);
	void UpdateGridRenderItems();
	void UpdateDebugGridText();
	void UpdateGridLayer();
	void UpdatePathLayer();

	int GetMouseX();
	int GetMouseY();
	int GetMouseCellX();
	int GetMouseCellY();
	int GetMapWidth();
	int GetMapHeight();
	int GetStartX();
	int GetStartY();
	int GetTargetX();
	int GetTargetY();
	const std::vector<RenderItem2D>& GetGridLayer();
	const std::vector<RenderItem2D>& GetPathLayer();
	const std::vector<RenderItem2D>& GetSelectorLayer();
}
