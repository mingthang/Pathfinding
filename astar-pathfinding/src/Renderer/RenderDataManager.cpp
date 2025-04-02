#include "RenderDataManager.h"
#include <Config/Config.h>
#include <Common/EngineDefines.h>
#include <Util/Util.h>
#include <Core/Game.h>
#include <Core/Input/Input.h>
#include <BackEnd/BackEnd.h>
#include <Core/GridMap/GridMap.h>
#include <span>
#include <unordered_map>
#include <AssetManager/AssetManager.h>

namespace RenderDataManager {
	RendererData g_rendererData;
    DrawCommandsSet g_drawCommandsSet;

    std::vector<RenderItem> g_renderItems;

    std::vector<RenderItem> g_instanceData;
	//std::vector<ViewportData> g_viewportData;

    void UpdateRendererData();
    void UpdateDrawCommandsSet();
    void SortRenderItems(std::vector<RenderItem>& renderItems);
    void CreateDrawCommands(DrawCommands& drawCommands, std::vector<RenderItem>& renderItem2Ds);
    void CreateMultiDrawIndirectCommands(std::vector<DrawIndexedIndirectCommand>& commands, std::span<RenderItem> renderItems, int playerIndex, int instanceOffset);

    void BeginFrame() {
        g_renderItems.clear();
    }

    void Update() {
        UpdateRendererData();
        UpdateDrawCommandsSet();
    }

    void UpdateRendererData() {
        const Resolutions& resolutions = Config::GetResolutions();
        g_rendererData.nearPlane = NEAR_PLANE;
        g_rendererData.farPlane = FAR_PLANE;
        g_rendererData.gBufferWidth = resolutions.gBuffer.x;
        g_rendererData.gBufferHeight = resolutions.gBuffer.y;
        g_rendererData.time = Game::GetTotalTime();
        g_rendererData.normalizedMouseX = Util::MapRange(Input::GetMouseX(), 0, BackEnd::GetCurrentWindowWidth(), 0.0f, 1.0f);
        g_rendererData.normalizedMouseY = Util::MapRange(Input::GetMouseY(), 0, BackEnd::GetCurrentWindowHeight(), 0.0f, 1.0f);
    }

    void UpdateDrawCommandsSet() {
        g_instanceData.clear();
        //auto& set = g_drawCommandsSet;

        //std::vector<RenderItem> renderItems;

        //CreateDrawCommands(set.geometry, renderItems);
    }

    void SortRenderItems(std::vector<RenderItem>& renderItems) {
        std::sort(renderItems.begin(), renderItems.end(), [](const RenderItem& a, const RenderItem& b) {
            return a.meshIndex < b.meshIndex;
            });
    }

    std::vector<RenderItem>& GetRenderItems()
    {
        return g_renderItems;
    }

    const RendererData& GetRendererData() {
        return g_rendererData;
    }

    void SubmitRenderItem(const RenderItem& renderItem) {
        g_renderItems.push_back(renderItem);
    }

    const DrawCommandsSet& GetDrawInfoSet() {
        return g_drawCommandsSet;
    }

}