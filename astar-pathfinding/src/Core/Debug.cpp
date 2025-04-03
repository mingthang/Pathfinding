#include "Debug.h"
#include <BackEnd/BackEnd.h>
#include <Core/Input/Input.h>
#include <Config/Config.h>
#include <iostream>
#include <Core/GridMap/GridMap.h>
#include <Core/Pathfinding/Pathfinding.h>
#include <Common/EngineTypes.h>
#include <Util/Util.h>
#include <API/OpenGL/Renderer/GL_renderer.h>

namespace Debug {
    std::string g_text = "";
    bool g_showDebugText = true;
    DebugRenderMode g_debugRenderMode = DebugRenderMode::NONE;

    void UpdateDebugPointsAndLines();
    void UpdateDebugText();

    void Update() {
        UpdateDebugPointsAndLines();
        UpdateDebugText();
    }

    void UpdateDebugText() {
        if (!g_showDebugText) return;

        const Resolutions& resolutions = Config::GetResolutions();

        int mouseX = Input::GetMouseX();
        int mouseY = Input::GetMouseY();
        int windowWidth = BackEnd::GetCurrentWindowWidth();
        int windowHeight = BackEnd::GetCurrentWindowHeight();
        int gBufferWidth = resolutions.gBuffer.x;
        int gBufferHeight = resolutions.gBuffer.y;

        AddText("");
        AddText("WindowWidth: " + std::to_string(windowWidth));
        AddText("WindowHeight: " + std::to_string(windowHeight));
        AddText("gBufferWidth: " + std::to_string(gBufferWidth));
        AddText("gBufferHeight: " + std::to_string(gBufferHeight));
        AddText("mouseX: " + std::to_string(mouseX));
        AddText("mouseY: " + std::to_string(mouseY));
    }

    void UpdateDebugPointsAndLines() {
        // Here check render mode: bones,  decals, ...

        const Resolutions resolutions = Config::GetResolutions();

        // Lines
		AStar aStar = Pathfinding::GetAStar();
		if (aStar.m_intersectionPoints.size() >= 2) {
			for (int i = 0; i < aStar.m_intersectionPoints.size() - 1; i++) {
				glm::vec2 cell0 = aStar.m_intersectionPoints[i];
				glm::vec2 cell1 = aStar.m_intersectionPoints[i + 1];
				Vertex2D begin = Vertex2D(Util::ScreenToNDC(glm::vec2(cell0.x * CELL_SIZE, cell0.y * CELL_SIZE), glm::vec2(resolutions.gBuffer.x, resolutions.gBuffer.y)), glm::vec4(1.0f));
                Vertex2D end = Vertex2D(Util::ScreenToNDC(glm::vec2(cell1.x * CELL_SIZE, cell1.y * CELL_SIZE), glm::vec2(resolutions.gBuffer.x, resolutions.gBuffer.y)), glm::vec4(1.0f));
                OpenGLRenderer::DrawLine(glm::vec3(begin.position, 0.0f), glm::vec3(end.position, 0.0f), begin.color);
			}
		}

        // Points
        for (int i = 0; i < aStar.m_intersectionPoints.size(); i++) {
            glm::vec2 cell0 = aStar.m_intersectionPoints[i];
            Vertex2D point = Vertex2D(Util::ScreenToNDC(glm::vec2(cell0.x * CELL_SIZE, cell0.y * CELL_SIZE), glm::vec2(resolutions.gBuffer.x, resolutions.gBuffer.y)), glm::vec4(1.0f));
            OpenGLRenderer::DrawPoint(glm::vec3(point.position, 0.0f), point.color);
        }

        OpenGLRenderer::UpdateDebugMesh();
    }

    void EndFrame() {
        g_text = "";
    }

    void AddText(const std::string& text) {
        g_text += text + "\n";
    }

    const std::string& GetText() {
        return g_text;
    }

    void ToggleDebugText() {
        g_showDebugText = !g_showDebugText;
    }

    bool IsDebugTextVisible() {
        return g_showDebugText;
    }
 }