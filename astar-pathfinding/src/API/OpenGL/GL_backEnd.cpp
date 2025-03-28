#include "GL_backEnd.h"
#include <iostream>

namespace OpenGLBackEnd {
    GLuint _vertexDataVAO = 0;
    GLuint _vertexDataVBO = 0;
    GLuint _vertexDataEBO = 0;

    void Init() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "GL_backEnd() failed to intialize GLAD.\n";
            return;
        }

        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);
        std::cout << "\nGPU: " << renderer << "\n";
        std::cout << "GL version: " << major << "." << minor << "\n\n";

        glClear(GL_COLOR_BUFFER_BIT);
    }

    GLuint GetVertexDataVAO() {
        return _vertexDataVAO;
    }

    GLuint GetVertexDataVBO() {
        return _vertexDataVBO;
    }

    GLuint GetVertexDataEBO() {
        return _vertexDataEBO;
    }
}