#include "GL_backEnd.h"
#include "GL_util.h"
#include <BackEnd/GLFWIntegration.h>
#include <AssetManager/BakeQueue.h>
#include <iostream>

namespace OpenGLBackEnd {
    const size_t MAX_TEXTURE_WIDTH = 4096;
    const size_t MAX_TEXTURE_HEIGHT = 4096;
    const size_t MAX_CHANNEL_COUNT = 4;
    const size_t MAX_DATA_SIZE = MAX_TEXTURE_WIDTH * MAX_TEXTURE_HEIGHT * MAX_CHANNEL_COUNT;

    GLuint _vertexDataVAO = 0;
    GLuint _vertexDataVBO = 0;
    GLuint _vertexDataEBO = 0;

    // CORE
    void Init() {

        GLFWIntegration::MakeContextCurrent();

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

    void BeginFrame() {
        return;
    }

    // Textures
    void AllocateTextureMemory(Texture& texture) {
        OpenGLTexture& glTexture = texture.GetGLTexture();
        GLuint& handle = glTexture.GetHandle();
        if (handle != 0) {
            return;
        }

        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OpenGLUtil::TextureWrapModeToGLEnum(texture.GetTextureWrapMode()));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OpenGLUtil::TextureWrapModeToGLEnum(texture.GetTextureWrapMode()));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, OpenGLUtil::TextureFilterToGLEnum(texture.GetMinFilter()));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, OpenGLUtil::TextureFilterToGLEnum(texture.GetMagFilter()));

        int mipmapWidth = texture.GetWidth(0);
        int mipmapHeight = texture.GetHeight(0);
        int levelCount = texture.MipmapsAreRequested() ? texture.GetMipmapLevelCount() : 1;
        for (int i = 0; i < levelCount; ++i) {
            if (texture.GetImageDataType() == ImageDataType::UNCOMPRESSED) {
                glTexImage2D(GL_TEXTURE_2D, i, texture.GetInternalFormat(), mipmapWidth, mipmapHeight, 0, texture.GetFormat(), GL_UNSIGNED_BYTE, nullptr);
            }
            else if (texture.GetImageDataType() == ImageDataType::COMPRESSED) {
                // ...
            }

            mipmapWidth = std::max(1, mipmapWidth / 2);
            mipmapHeight = std::max(1, mipmapHeight / 2);
        }

        // Instead of explicitly binding a texture to a texture unit, 
        // bindless textures allow us to manually mark the texture as resident on the GPU 
        // and then pass its handle to a shader using a uniform buffer or shader storage buffer.
        glTexture.MakeBindlessTextureResident();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void ImmediateBake(QueuedTextureBake& queuedTextureBake) {
        Texture* texture = static_cast<Texture*>(queuedTextureBake.texture);
        OpenGLTexture& glTexture = texture->GetGLTexture();
        int width = queuedTextureBake.width;
        int height = queuedTextureBake.height;
        int format = queuedTextureBake.format;
        int internalFormat = queuedTextureBake.internalFormat;
        int level = queuedTextureBake.mipmapLevel;
        int dataSize = queuedTextureBake.dataSize;
        const void* data = queuedTextureBake.data;

        GLuint textureHandle = glTexture.GetHandle();

        // Bake texture data
        if (texture->GetImageDataType() == ImageDataType::UNCOMPRESSED) {
            glTextureSubImage2D(textureHandle, level, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
        }
        else if (texture->GetImageDataType() == ImageDataType::EXR) {
            //glTextureSubImage2D(textureHandle, 0, 0, 0, glTexture.GetWidth(), glTexture.GetHeight(), GL_RGBA, GL_FLOAT, glTexture.GetData());
        }
        else if (texture->GetImageDataType() == ImageDataType::COMPRESSED) {
            //glCompressedTextureSubImage2D(textureHandle, level, 0, 0, width, height, internalFormat, dataSize, data);
        }

        texture->SetTextureDataLevelBakeState(level, BakeState::BAKE_COMPLETE);

        // Generate Mipmaps if none were supplied
        if (texture->MipmapsAreRequested()) {
            if (texture->GetTextureDataCount() == 1) {
                glGenerateTextureMipmap(textureHandle);
            }
        }
        // Cleanup bake queue
        BakeQueue::RemoveQueuedTextureBakeByJobID(queuedTextureBake.jobID);
    }

    // GET
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