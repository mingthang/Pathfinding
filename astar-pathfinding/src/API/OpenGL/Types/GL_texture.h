#pragma once

#include <Common/EngineTypes.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

struct OpenGLTexture {
public:
	OpenGLTexture() = default;

    void AllocateMemory(int width, int height, int format, int internalFormat, int mipmapLevelCount);
    void MakeBindlessTextureResident();
    void MakeBindlessTextureNonResident();
    
    // SET
    void SetWrapMode(TextureWrapMode wrapMode);
    void SetMinFilter(TextureFilter filter);
    void SetMagFilter(TextureFilter filter);

    // GET
	GLuint& GetHandle();
    GLuint64 GetBindlessID();
    int GetWidth();
    int GetHeight();
    int GetChannelCount();
    int GetDataSize();
    void* GetData();
    GLint GetFormat();
    GLint GetInternalFormat();
    GLint GetMipmapLevelCount();

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
