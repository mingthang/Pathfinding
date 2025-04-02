#include "Game.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Common/EngineDefines.h>
#include <Core/Pathfinding/Pathfinding.h>
#include <Config/Config.h>
#include <Core/GridMap/GridMap.h>

namespace Game {
	float g_deltaTime = 0;
	float g_totalTime = 0;
	double g_deltaTimeAccumulator = 0.0;
	double g_fixedDeltaTime = 1.0 / 60.0;

	void UpdateLazyKeypresses();

	void Create() {
		GridMap::Init();
	}

	void BeginFrame() {
		return;
	}

	void Update() {
		static double lastTime = glfwGetTime(); // lazy initialization
		double currentTime = glfwGetTime();
		g_deltaTime = static_cast<float>(currentTime - lastTime);
		lastTime = currentTime;
		g_deltaTimeAccumulator += g_deltaTime;

		g_totalTime += g_deltaTime;
		if (g_totalTime > TIME_WRAP) {
			g_totalTime -= TIME_WRAP; // Keep it continuous
		}

		GridMap::Update(g_deltaTime);
	}

	float GetDeltaTime() {
		return g_deltaTime;
	}

	float GetTotalTime() {
		return g_totalTime;
	}
}
