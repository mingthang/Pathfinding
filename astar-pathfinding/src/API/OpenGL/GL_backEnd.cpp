#include "GL_backEnd.h"
#include "GL_util.h"
#include <BackEnd/GLFWIntegration.h>
#include <AssetManager/BakeQueue.h>
#include <iostream>
#include "Types/GL_pbo.h"

namespace OpenGLBackEnd {
    const size_t MAX_TEXTURE_WIDTH = 32;
    const size_t MAX_TEXTURE_HEIGHT = 32;
    const size_t MAX_CHANNEL_COUNT = 4;
    const size_t MAX_DATA_SIZE = MAX_TEXTURE_WIDTH * MAX_TEXTURE_HEIGHT * MAX_CHANNEL_COUNT;
    std::vector<PBO> g_textureBakingPBOs;

    GLuint g_vertexDataVAO = 0;
    GLuint g_vertexDataVBO = 0;
    GLuint g_vertexDataEBO = 0;

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

        // Texture baking PBOs
        for (int i = 0; i < 2; ++i) {
            PBO& pbo = g_textureBakingPBOs.emplace_back();
            pbo.Init(MAX_DATA_SIZE);
        }
    }

    void BeginFrame() {
        return;
    }

    void UploadVertexData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {

        if (g_vertexDataVAO != 0) {
            glDeleteVertexArrays(1, &g_vertexDataVAO);
            glDeleteBuffers(1, &g_vertexDataVBO);
            glDeleteBuffers(1, &g_vertexDataEBO);
        }

        glGenVertexArrays(1, &g_vertexDataVAO);
        glGenBuffers(1, &g_vertexDataVBO);
        glGenBuffers(1, &g_vertexDataEBO);

        glBindVertexArray(g_vertexDataVAO);
        glBindBuffer(GL_ARRAY_BUFFER, g_vertexDataVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_vertexDataEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
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

    void UpdateTextureBaking() {
        size_t bakeCommandsIssuedPerFrame = g_textureBakingPBOs.size();

        for (size_t i = 0; i < bakeCommandsIssuedPerFrame; i++) {
            // Update PBO states
            for (PBO& pbo : g_textureBakingPBOs) {
                pbo.UpdateState();
            }

            // If any have completed, remove the job ID from the queue
            for (PBO& pbo : g_textureBakingPBOs) {
                uint32_t jobID = pbo.GetCustomValue();
                if (pbo.IsSyncComplete() && jobID != -1) {
                    QueuedTextureBake* queuedTextureBake = BakeQueue::GetQueuedTextureBakeByJobID(jobID);
                    Texture* texture = static_cast<Texture*>(queuedTextureBake->texture);
                    texture->SetTextureDataLevelBakeState(queuedTextureBake->mipmapLevel, BakeState::BAKE_COMPLETE);

                    // Generate mipmaps if none were supplied
                    if (texture->MipmapsAreRequested()) {
                        if (texture->GetTextureDataCount() == 1) {
                            glGenerateTextureMipmap(texture->GetGLTexture().GetHandle());
                        }
                    }
                    BakeQueue::RemoveQueuedTextureBakeByJobID(jobID);
                    pbo.SetCustomValue(-1);
                }
            }

            // Bake the next queued texture bake (if one exists)
            if (BakeQueue::GetQueuedTextureBakeJobCount() > 0) {
                QueuedTextureBake* queuedTextureBake = BakeQueue::GetNextQueuedTextureBake();
                if (queuedTextureBake) {
                    AsyncBakeQueuedTextureBake(*queuedTextureBake);
                }
            }
        }
    }

    void AsyncBakeQueuedTextureBake(QueuedTextureBake& queuedTextureBake) {
        // Get next free PBO
        PBO* pbo = nullptr;
        for (PBO& queryPbo : g_textureBakingPBOs) {
            if (queryPbo.IsSyncComplete()) {
                pbo = &queryPbo;
                break;
            }
        }

        // Return early if no free PBOs
        if (!pbo) {
            std::cerr << "Warning: Attempting to use an active PBO!" << std::endl;
            return;
        }

        queuedTextureBake.inProgress = true;

        Texture* texture = static_cast<Texture*>(queuedTextureBake.texture);
        int jobID = queuedTextureBake.jobID;
        int width = queuedTextureBake.width;
        int height = queuedTextureBake.height;
        int format = queuedTextureBake.format;
        int internalFormat = queuedTextureBake.internalFormat;
        int level = queuedTextureBake.mipmapLevel;
        int dataSize = queuedTextureBake.dataSize;
        const void* data = queuedTextureBake.data;

        texture->SetTextureDataLevelBakeState(level, BakeState::BAKING_IN_PROGRESS);

        // Map PBO and copy data
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo->GetHandle());
        std::memcpy(pbo->GetPersistentBuffer(), data, dataSize);

        // Upload data to the texture using DSA
        GLuint textureHandle = texture->GetGLTexture().GetHandle();
        if (texture->GetImageDataType() == ImageDataType::UNCOMPRESSED) {
            glTextureSubImage2D(textureHandle, level, 0, 0, width, height, format, GL_UNSIGNED_BYTE, 0);
        }
        else if (texture->GetImageDataType() == ImageDataType::COMPRESSED) {
            glCompressedTextureSubImage2D(textureHandle, level, 0, 0, width, height, internalFormat, dataSize, 0);
        }
        else if (texture->GetImageDataType() == ImageDataType::EXR) {
            // Handle EXR case (example left as a placeholder)
            // glTextureSubImage2D(textureHandle, level, 0, 0, width, height, format, GL_FLOAT, 0);
        }

        // Start PBO sync and assign job ID
        pbo->SyncStart();
        pbo->SetCustomValue(jobID);

        // Unbind PBO
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }

    void CleanUpBakingPBOs() {
        for (PBO& pbo : g_textureBakingPBOs) {
            pbo.CleanUp();
        }
        g_textureBakingPBOs.clear();
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
        return g_vertexDataVAO;
    }

    GLuint GetVertexDataVBO() {
        return g_vertexDataVBO;
    }

    GLuint GetVertexDataEBO() {
        return g_vertexDataEBO;
    }
}