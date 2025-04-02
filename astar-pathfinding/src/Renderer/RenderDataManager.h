#pragma once

#include <Common/EngineTypes.h>

namespace RenderDataManager {
	void BeginFrame();
	void Update();

	std::vector<RenderItem>& GetRenderItems();

	const RendererData& GetRendererData();
	const DrawCommandsSet& GetDrawInfoSet();
}
