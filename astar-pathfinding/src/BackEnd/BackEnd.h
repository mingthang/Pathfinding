#pragma once

#include <Common/EngineEnums.h>

namespace BackEnd {
	// Core
	bool Init(WindowedMode windowedMode);
	bool WindowIsOpen();
	void BeginFrame();
	void UpdateGame();
	void UpdateSubSystems();
	void EndFrame();
	void CleanUp();
}
