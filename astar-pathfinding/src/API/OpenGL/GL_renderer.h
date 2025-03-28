#pragma once

namespace OpenGLRenderer {
	void Init();
	void RenderFrame(RenderData& renderData);
	void HotloadShaders();
	void CreateRenderTargets(int presentWidth, int presentHeight);
	void PresentFinalImage();
}
