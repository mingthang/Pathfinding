#pragma once

#include <Common/EngineEnums.h>

namespace BackEnd {
	// Core
	bool Init(WindowedMode windowedMode);
	bool WindowIsOpen();
	void BeginFrame();
	void EndFrame();
	void CleanUp();
}
