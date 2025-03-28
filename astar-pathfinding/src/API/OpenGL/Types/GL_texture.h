#pragma once

#include <Common/EngineTypes.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

struct OpenGLTexture {
public:
	OpenGLTexture() = default;

	GLuint& GetHandle();
    void AllocateMemory(int width, int height, int format, int internalFormat, int mipmapLevelCount);

private:
    GLuint m_handle = 0;
    GLuint64 m_bindlessID = 0;
    int m_width = 0;
    int m_height = 0;
    int m_channelCount = 0;
    GLsizei m_dataSize = 0;
    void* m_data = nullptr;
    GLint m_format = 0;
    GLint m_internalFormat = 0;
    GLint m_mipmapLevelCount = 0;
    ImageDataType m_imageDataType;
    bool m_memoryAllocated = false;
};
