#include "GL_renderer.h"
#include <API/OpenGL/Types/GL_shader.h>
#include <API/OpenGL/Types/GL_framebuffer.h>
#include "API/OpenGL/Types/GL_indirectBuffer.h"
#include <Config/Config.h>
#include <Common/EngineDefines.h>

namespace OpenGLRenderer {

	std::unordered_map<std::string, OpenGLShader> g_shaders;
	std::unordered_map<std::string, OpenGLFrameBuffer> g_frameBuffers;
	IndirectBuffer g_indirectBuffer;

	void LoadShaders();
	void ClearRenderTargets();

	void Init() {
		const Resolutions& resolutions = Config::GetResolutions();

        // G-Buffer
        g_frameBuffers["GBuffer"] = OpenGLFrameBuffer("GBuffer", resolutions.gBuffer);
        g_frameBuffers["GBuffer"].CreateAttachment("BaseColor", GL_RGBA8);
        g_frameBuffers["GBuffer"].CreateDepthAttachment(GL_DEPTH_COMPONENT32F);

		g_frameBuffers["UI"] = OpenGLFrameBuffer("UI", resolutions.ui);
		g_frameBuffers["UI"].CreateAttachment("Color", GL_RGBA8, GL_NEAREST, GL_NEAREST);

		g_frameBuffers["FinalImage"] = OpenGLFrameBuffer("FinalImage", resolutions.finalImage);
		g_frameBuffers["FinalImage"].CreateAttachment("Color", GL_RGBA16F);

		// Preallocate the indirect command buffer
		g_indirectBuffer.PreAllocate(sizeof(DrawIndexedIndirectCommand) * MAX_INDIRECT_DRAW_COMMAND_COUNT);

		LoadShaders();
	}

	void InitMain() {
		return;
	}

	void LoadShaders() {
		g_shaders["UI"] = OpenGLShader({ "GL_ui.vert", "GL_ui.frag" });
		g_shaders["DebugVertex"] = OpenGLShader({ "GL_debug_vertex.vert", "GL_debug_vertex.frag" });
		g_shaders["Tile"] = OpenGLShader({ "GL_tile.vert", "GL_tile.frag" });
	}

	void RenderGame() {
		glDisable(GL_DITHER);

		ClearRenderTargets();

		GridPass();
		DebugPass();

		OpenGLFrameBuffer& gBuffer = g_frameBuffers["GBuffer"];
		OpenGLFrameBuffer& finalImageBuffer = g_frameBuffers["FinalImage"];

		OpenGLRenderer::BlitFrameBuffer(&gBuffer, &finalImageBuffer, "BaseColor", "Color", GL_COLOR_BUFFER_BIT, GL_LINEAR);
		OpenGLRenderer::BlitToDefaultFrameBuffer(&finalImageBuffer, "Color", GL_COLOR_BUFFER_BIT, GL_NEAREST);

		UIPass();
	}

	void ClearRenderTargets() {
		OpenGLFrameBuffer* gBuffer = GetFrameBuffer("GBuffer");

		// GBuffer
		glDepthMask(GL_TRUE);
		gBuffer->ClearAttachment("BaseColor", 0.0f, 0.0f, 0.0f, 1.0f);
		gBuffer->ClearDepthAttachment();
	}

	void MultiDrawIndirect(const std::vector<DrawIndexedIndirectCommand>& commands) {
		if (commands.size()) {
			// Feed the draw command data to the gpu
			g_indirectBuffer.Bind();
			g_indirectBuffer.Update(sizeof(DrawIndexedIndirectCommand) * commands.size(), commands.data());

			// Fire of the commands
			glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)0, (GLsizei)commands.size(), 0);
		}
	}

	OpenGLShader* GetShader(const std::string& name) {
		auto it = g_shaders.find(name);
		return (it != g_shaders.end()) ? &it->second : nullptr;
	}

	OpenGLFrameBuffer* GetFrameBuffer(const std::string& name) {
		auto it = g_frameBuffers.find(name);
		return (it != g_frameBuffers.end()) ? &it->second : nullptr;
	}

}