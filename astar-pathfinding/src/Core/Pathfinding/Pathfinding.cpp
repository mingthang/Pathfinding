#include "Pathfinding.h"

#include <Core/Input/Input.h>
#include <Config/Config.h>
#include <algorithm>

namespace Pathfinding {
    AStar g_AStar;
    bool g_slowMode = true;
    bool g_allowDiagonal = false;
    bool g_biDirectional = false;
    bool g_crossCorners = false;

    AStar& GetAStar() {
        return g_AStar;
    }

    void ResetAStar() {
        g_AStar.ClearData();
    }

    void ToggleSlowMode() {
        g_slowMode = !g_slowMode;
    }

    bool SlowModeEnabled() {
        return g_slowMode;
    }
}

void AStar::InitSearch(std::vector<std::vector<bool>>& map, int startX, int startY, int destinationX, int destinationY) {
    ClearData();
    m_cells.resize(GridMap::GetMapWidth(), std::vector<Cell>(GridMap::GetMapHeight()));
    m_openList.AllocateSpace(GridMap::GetMapWidth() * GridMap::GetMapHeight());
    m_openList.Clear();
    for (int x = 0; x < GridMap::GetMapWidth(); x++) {
        for (int y = 0; y < GridMap::GetMapHeight(); y++) {
            m_cells[x][y].x = x;
            m_cells[x][y].y = y;
            m_cells[x][y].obstacle = GridMap::IsObstacle(x, y);
            m_cells[x][y].g = 99999;
            m_cells[x][y].h = -1;
            m_cells[x][y].f = -1;
            m_cells[x][y].parent = nullptr;
            m_cells[x][y].neighbours.clear();
        }
    }
    m_start = &m_cells[startX][startY];
    m_current = m_start;
    m_destination = &m_cells[destinationX][destinationY];
    m_start->g = 0;
    m_start->GetF(m_destination);
    m_openList.AddItem(m_start);
    m_searchInitilized = true;
}

void AStar::ClearData() {
    m_closedList.clear();
    m_finalPath.clear();
    m_openList.Clear();
    m_intersectionPoints.clear();
    m_gridPathFound = false;
    m_smoothPathFound = false;
    m_searchInitilized = false;
    m_smoothSearchIndex = 0;
}

bool AStar::GridPathFound() {
    return m_gridPathFound;
}

bool AStar::SmoothPathFound() {
    return m_smoothPathFound;
}

bool AStar::SearchInitilized() {
    return m_searchInitilized;
}

void AStar::FindPath() {
    if (m_destination->obstacle) {
        return;
    }
    if (m_openList.IsEmpty()) {
        return;
    }
    if (m_gridPathFound) {
        return;
    }
    //while (!m_openList.IsEmpty()
    {
        m_current = m_openList.RemoveFirst();
        if (IsDestination(m_current)) {
            m_gridPathFound = true;
            BuildFinalPath();
            return;
        }
        AddIfUnique(&m_closedList, m_current);
        FindNeighbours(m_current);
        for (Cell* neighbour : m_current->neighbours) {
            // Calculate G cost. Equal to parent G cost + 10 if orthogonal and + 14 if diagonal
            int new_g = IsOrthogonal(m_current, neighbour) ? m_current->g + ORTHOGONAL_COST : m_current->g + DIAGONAL_COST;

            if (IsInClosedList(neighbour) || m_openList.Contains(neighbour)) {
                // If new G is lower than currently stored value, update it and change parent to the current cell
                if (new_g < neighbour->g) {
                    neighbour->g = new_g;
                    neighbour->f = new_g + neighbour->GetH(m_destination);
                    neighbour->parent = m_current;
                }
            }
            else {
                neighbour->g = new_g;
                neighbour->f = new_g + neighbour->GetH(m_destination);
                neighbour->parent = m_current;

                if (!m_openList.Contains(neighbour)) {
                    m_openList.AddItem(neighbour);
                }
            }
        }
    }
    if (!Pathfinding::SlowModeEnabled()) {
        FindPath();
    }
}

MinHeap& AStar::GetOpenList() {
    return m_openList;
}

std::list<Cell*>& AStar::GetClosedList() {
    return m_closedList;
}

std::vector<Cell*>& AStar::GetPath() {
    return m_finalPath;
}

bool AStar::IsDestination(Cell* cell) {
    return cell == m_destination;
}

void AStar::BuildFinalPath() {
    Cell* cell = m_destination;
    while (cell != m_start) {
        m_finalPath.push_back(cell);
        cell = cell->parent;
    }
    std::reverse(m_finalPath.begin(), m_finalPath.end());

    // Init smooth search
    m_smoothPathFound = false;
    m_smoothSearchIndex = 2;
    m_intersectionPoints.clear();
    glm::vec2 startPoint = glm::vec2(m_start->x + 0.5f, m_start->y + 0.5f);
    glm::vec2 endPoint = glm::vec2(m_destination->x + 0.5f, m_destination->y + 0.5f);
    m_intersectionPoints.push_back(startPoint);
    for (int j = 0; j < m_finalPath.size(); j++) {
        Cell* cell = m_finalPath[j];
        m_intersectionPoints.push_back(glm::vec2(cell->x, cell->y));
    }
    m_intersectionPoints.push_back(endPoint);
}

bool HasLineOfSight(glm::vec2 startPosition, glm::vec2 endPosition) {
    float stepSize = 0.5f;
    glm::vec2 direction = glm::normalize(endPosition - startPosition);
    glm::vec2 testPosition = startPosition;
    float distanceToEnd = glm::distance(testPosition, endPosition);
    float currentDistance = distanceToEnd;
    while (currentDistance > 1) {
        testPosition += direction * stepSize;
        if (GridMap::IsObstacle((int)testPosition.x, (int)testPosition.y)) {
            return false;
        }
        currentDistance = glm::distance(testPosition, endPosition);
        // You made it past your target! There is line of sight
        if (currentDistance > distanceToEnd) {
            return true;
        }
    }
    return true;
}

void AStar::FindSmoothPath() {
    if (!m_gridPathFound) {
        return;
    }
    if (m_smoothSearchIndex >= m_intersectionPoints.size()) {
        m_smoothPathFound = true;
    }
    if (m_smoothPathFound) {
        return;
    }
    // Remove points with line of sight
    glm::vec2 currentPosition = m_intersectionPoints[m_smoothSearchIndex];
    glm::vec2 queryPosition = m_intersectionPoints[m_smoothSearchIndex - 2];
    glm::vec2 intersectionPointOut;
    if (HasLineOfSight(currentPosition, queryPosition)) {
        m_intersectionPoints.erase(m_intersectionPoints.begin() + m_smoothSearchIndex - 1);
    }
    else {
        m_smoothSearchIndex++;
    }
    if (!Pathfinding::SlowModeEnabled()) {
        FindSmoothPath();
    }
}

void AStar::AddIfUnique(std::list<Cell*>* list, Cell* cell) {
    std::list<Cell*>::iterator it;
    it = std::find(list->begin(), list->end(), cell);
    if (it == list->end()) {
        list->push_front(cell);
    }
}

bool AStar::IsOrthogonal(Cell* cellA, Cell* cellB) {
    return (cellA->x == cellB->x || cellA->y == cellB->y);
}

bool AStar::IsInClosedList(Cell* cell) {
    std::list<Cell*>::iterator it;
    it = std::find(m_closedList.begin(), m_closedList.end(), cell);
    return (it != m_closedList.end());
}

void AStar::FindNeighbours(Cell* cell) {
    if (m_current->neighbours.size() != 0) {
        return;
    }
    int x = cell->x;
    int y = cell->y;
    // North
    if (GridMap::IsInBounds(x, y - 1) && !GridMap::IsObstacle(x, y - 1)) {
        m_cells[x][y].neighbours.push_back(&m_cells[x][y - 1]);
    }
    // South
    if (GridMap::IsInBounds(x, y + 1) && !GridMap::IsObstacle(x, y + 1)) {
        m_cells[x][y].neighbours.push_back(&m_cells[x][y + 1]);
    }
    // West
    if (GridMap::IsInBounds(x - 1, y) && !GridMap::IsObstacle(x - 1, y)) {
        m_cells[x][y].neighbours.push_back(&m_cells[x - 1][y]);
    }
    // East
    if (GridMap::IsInBounds(x + 1, y) && !GridMap::IsObstacle(x + 1, y)) {
        m_cells[x][y].neighbours.push_back(&m_cells[x + 1][y]);
    }
    /*
    // North West
    if (Pathfinding::IsInBounds(x - 1, y - 1) && !Pathfinding::IsObstacle(x - 1, y - 1)) {// && !Pathfinding::IsObstacle(x - 1, y) && !Pathfinding::IsObstacle(x, y - 1)) {
        m_cells[x][y].neighbours.push_back(&m_cells[x - 1][y - 1]);
    }
    // North East
    if (Pathfinding::IsInBounds(x + 1, y - 1) && !Pathfinding::IsObstacle(x + 1, y - 1)) {// && !Pathfinding::IsObstacle(x + 1, y) && !Pathfinding::IsObstacle(x, y - 1)) {
        m_cells[x][y].neighbours.push_back(&m_cells[x + 1][y - 1]);
    }
    // South West
    if (Pathfinding::IsInBounds(x - 1, y + 1) && !Pathfinding::IsObstacle(x - 1, y + 1)) {// && !Pathfinding::IsObstacle(x - 1, y) && !Pathfinding::IsObstacle(x, y + 1)) {
        m_cells[x][y].neighbours.push_back(&m_cells[x - 1][y + 1]);
    }
    // South East
    if (Pathfinding::IsInBounds(x + 1, y + 1) && !Pathfinding::IsObstacle(x + 1, y + 1)) {// && !Pathfinding::IsObstacle(x + 1, y) && !Pathfinding::IsObstacle(x, y + 1)) {
        m_cells[x][y].neighbours.push_back(&m_cells[x + 1][y + 1]);
    }*/
}

void MinHeap::AllocateSpace(int size) {
    items = std::vector<Cell*>(size);
}

void MinHeap::AddItem(Cell* item) {
    item->heapIndex = currentItemCount;
    items[currentItemCount] = item;
    SortUp(item);
    currentItemCount++;
}

void MinHeap::Update(Cell* cell) {
    SortUp(cell);
}

bool MinHeap::Contains(Cell* cell) {
    return (items[cell->heapIndex] == cell);
}

bool MinHeap::IsEmpty() {
    return (currentItemCount == 0);
}

int MinHeap::Size() {
    return currentItemCount;
}

void MinHeap::Clear() {
    currentItemCount = 0;
}

void MinHeap::SortUp(Cell* item) {
    int parentIndex = (item->heapIndex - 1) / 2;
    while (true) {
        Cell* parent = items[parentIndex];
        if (parent->f > item->f) {
            Swap(item, parent);
        }
        else {
            break;
        }
        parentIndex = (item->heapIndex - 1) / 2;
    }
}

void MinHeap::Swap(Cell* cellA, Cell* cellB) {
    items[cellA->heapIndex] = cellB;
    items[cellB->heapIndex] = cellA;
    int itemAIndex = cellA->heapIndex;
    cellA->heapIndex = cellB->heapIndex;
    cellB->heapIndex = itemAIndex;
}

Cell* MinHeap::RemoveFirst() {
    Cell* firstItem = items[0];
    currentItemCount--;
    items[0] = items[currentItemCount];
    items[0]->heapIndex = 0;
    SortDown(items[0]);
    return firstItem;
}

void MinHeap::SortDown(Cell* cell) {
    while (true) {
        int childIndexLeft = cell->heapIndex * 2 + 1;
        int childIndexRight = cell->heapIndex * 2 + 2;
        int swapIndex = 0;
        if (childIndexLeft < currentItemCount) {
            swapIndex = childIndexLeft;
            if (childIndexRight < currentItemCount) {
                if (items[childIndexLeft]->f > items[childIndexRight]->f) {
                    swapIndex = childIndexRight;
                }
            }
            if (cell->f > items[swapIndex]->f)
                Swap(cell, items[swapIndex]);
            else {
                return;
            }
        }
        else {
            return;
        }
    }
}