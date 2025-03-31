#include "GL_renderer.h"
#include <API/OpenGL/Types/GL_shader.h>
#include <API/OpenGL/Types/GL_framebuffer.h>
#include <Config/Config.h>

namespace OpenGLRenderer {

	std::unordered_map<std::string, OpenGLShader> g_shaders;
	std::unordered_map<std::string, OpenGLFrameBuffer> g_frameBuffers;

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

		LoadShaders();
	}

	void InitMain() {
		return;
	}

	void LoadShaders() {
		g_shaders["UI"] = OpenGLShader({ "GL_ui.vert", "GL_ui.frag" });
	}

	void RenderGame() {
		glDisable(GL_DITHER);

		ClearRenderTargets();

		//DebugPass();
		UIPass();
	}

	void ClearRenderTargets() {
		OpenGLFrameBuffer* gBuffer = GetFrameBuffer("GBuffer");

		// GBuffer
		glDepthMask(GL_TRUE);
		gBuffer->ClearAttachment("BaseColor", 0, 0, 0, 0);
		gBuffer->ClearDepthAttachment();
		//gBuffer->ClearAttachment("Normal", 0, 0, 0, 0);
		//gBuffer->ClearAttachment("RMA", 0, 0, 0, 0);
		//gBuffer->ClearAttachmentUI("MousePick", 0, 0);
		//gBuffer->ClearAttachment("WorldSpacePosition", 0, 0);
		//gBuffer->ClearAttachment("Emissive", 0, 0, 0, 0);
		//gBuffer->ClearAttachment("Glass", 0, 1, 0, 0);
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