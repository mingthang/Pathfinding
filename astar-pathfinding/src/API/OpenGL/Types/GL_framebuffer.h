#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Util/Util.h>
#include <iostream>

struct ColorAttachment {
    const char* name = "undefined";
    GLuint handle = 0;
    GLenum internalFormat = 0;;
    GLenum format = 0;
    GLenum type = 0;
};

struct DepthAttachment {
    GLuint handle = 0;
    GLenum internalFormat = 0;
};

class OpenGLFrameBuffer {

private:
    const char* m_name = "undefined";
    GLuint m_handle = 0;
    GLuint m_width = 0;
    GLuint m_height = 0;
    std::vector<ColorAttachment> m_colorAttachments;
    DepthAttachment m_depthAttachment;

public:

    OpenGLFrameBuffer() = default;
    OpenGLFrameBuffer(const char* name, int width, int height);
    OpenGLFrameBuffer(const char* name, const glm::ivec2& resolution);

    void Create(const char* name, int width, int height);
    void Create(const char* name, const glm::ivec2& resolution);
    void CreateAttachment(const char* name, GLenum internalFormat, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR);
    void CreateDepthAttachment(GLenum internalFormat, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR, GLint wrap = GL_CLAMP_TO_EDGE, glm::vec4 borderColor = glm::vec4(1.0f));
    void Bind();
    void BindDepthAttachmentFrom(const OpenGLFrameBuffer& srcFrameBuffer);
    void SetViewport();
    void CleanUp();

    void ClearAttachment(const char* attachmentName, GLfloat r, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 0.0f);

    void DrawBuffers(std::vector<const char*> attachmentNames);
    void DrawBuffer(const char* attachmentName);

    void Resize(int width, int height);
    GLuint GetHandle() const;
    GLuint GetWidth() const;
    GLuint GetHeight() const;
    GLuint GetColorAttachmentHandleByName(const char* name) const;
    GLuint GetDepthAttachmentHandle() const;
    GLenum GetColorAttachmentSlotByName(const char* name) const;
};
