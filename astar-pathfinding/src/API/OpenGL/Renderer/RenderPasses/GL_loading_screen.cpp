#include <API/OpenGL/Renderer/GL_renderer.h>
#include <UI/UIBackEnd.h>

namespace OpenGLRenderer {

    void RenderLoadingScreen() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render UI
        UIBackEnd::Update();
        UIPass();
    }
}