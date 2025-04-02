#pragma once

#include <Common/EngineEnums.h>
#include <Common/EngineTypes.h>

namespace Game {
	void Create();
	void BeginFrame();
	void Update();

	float GetDeltaTime();
	float GetTotalTime();
}
