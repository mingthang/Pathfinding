#include "GL_renderer.h"
#include "Types/GL_shader.h"
#include "Types/GL_framebuffer.h"
#include <Renderer/RendererCommon.h>

namespace OpenGLRenderer {

    struct FrameBuffers {
        GLFrameBuffer present;
    } g_frameBuffers;

    struct Shaders {
        Shader UI;
        Shader debugSolidColor;
    } g_shaders;

    struct SSBOs {
        GLuint samplers = 0;
        GLuint renderItems2D = 0;

    } g_ssbos;

	void Init() {
        // HotloadShaders();
        glCreateBuffers(1, &g_ssbos.renderItems2D);
        glNamedBufferStorage(g_ssbos.renderItems2D, MAX_RENDER_OBJECTS_2D * sizeof(RenderItem2D), NULL, GL_DYNAMIC_STORAGE_BIT);
	}

    void CreateRenderTargets(int presentWidth, int presentHeight) {
        g_frameBuffers.present.Create("Present", presentWidth, presentHeight);
        g_frameBuffers.present.CreateAttachment("Color", GL_RGBA8);
        g_frameBuffers.present.CreateDepthAttachment(GL_DEPTH32F_STENCIL8);
    }


}