#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EngineEnums.h"
#include <vector>
#include <string>

struct RenderItem {
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::mat4 inverseModelMatrix = glm::mat4(1);
    glm::vec4 aabbMin = glm::vec4(0);
    glm::vec4 aabbMax = glm::vec4(0);

    int meshIndex = 0;
    int baseColorTextureIndex = 0;
    int normalMapTextureIndex = 0;
    int rmaTextureIndex = 0;

    int objectType = 0;
    int mousePickIndex = 0;
    int baseSkinnedVertex = 0;
    int ignoredViewportIndex = -1;

    int exclusiveViewportIndex = -1;
    int skinned = 0;    // True or false
    float emissiveR = 0.0f;
    float emissiveG = 0.0f;

    float emissiveB = 0.0f;
    float padding0 = 0.0f;
    float padding1 = 0.0f;
    float padding2 = 0.0f;
};

struct RenderItem2D {
    glm::mat4 modelMatrix = glm::mat4(1);
    float colorTintR = 1.0f;
    float colorTintG = 1.0f;
    float colorTintB = 1.0f;
    int textureIndex = -1;
    int baseVertex = 0;
    int baseIndex = 0;
};

struct Transform {
    glm::vec3 position = glm::vec3(0);
    glm::vec3 rotation = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1);

    Transform() = default;

    explicit Transform(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f)) {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
    }

    glm::mat4 to_mat4() const {
        glm::mat4 m = glm::translate(glm::mat4(1), position);
        m *= glm::mat4_cast(glm::quat(rotation));
        m = glm::scale(m, scale);
        return m;
    };
};

struct Vertex2D {
    Vertex2D() = default;
    Vertex2D(glm::vec2 pos) {
        position = pos;
    }
    Vertex2D(glm::vec2 pos, glm::vec2 texCoord) {
        position = pos;
        uv = texCoord;
    }

    Vertex2D(glm::vec2 pos, glm::vec2 texCoord, glm::vec4 color) {
        position = pos;
        uv = texCoord;
        this->color = color;
    }

    Vertex2D(glm::vec2 pos, glm::vec4 color) {
        position = pos;
        this->color = color;
    }

    Vertex2D(glm::vec2 pos, glm::vec2 texCoord, glm::vec4 color, int textureIndex) {
        position = pos;
        uv = texCoord;
        this->color = color;
        this->textureIndex = textureIndex;
    }

    glm::vec2 position = glm::vec2(0);
    glm::vec2 uv = glm::vec2(0);
    glm::vec4 color = glm::vec4(0);
    int textureIndex;
};

struct Vertex {
    Vertex() = default;
    Vertex(glm::vec3 pos) {
        position = pos;
    }
    Vertex(glm::vec3 pos, glm::vec3 norm) {
        position = pos;
        normal = norm;
    }
    Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texCoord) {
        position = pos;
        normal = norm;
        uv = texCoord;
    }
    Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texCoord, glm::vec3 _tangent) {
        position = pos;
        normal = norm;
        uv = texCoord;
        tangent = _tangent;
    }
    glm::vec3 position = glm::vec3(0);
    glm::vec3 normal = glm::vec3(0);
    glm::vec2 uv = glm::vec2(0);
    glm::vec3 tangent = glm::vec3(0);
};

#pragma pack(push, 1)
struct DebugVertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::ivec2 pixelOffset;
    int exclusiveViewportIndex = -1;
};
#pragma pack(pop)

struct RendererData {
    float nearPlane;
    float farPlane;
    float gBufferWidth;
    float gBufferHeight;

    float time;
    //int splitscreenMode;

    int rendererOverrideState;
    float normalizedMouseX;
    float normalizedMouseY;
};

struct TextureData {
    int m_width = 0;
    int m_height = 0;
    int m_channelCount = 0;
    int m_dataSize = 0;
    int m_format = 0;
    int m_internalFormat = 0;
    void* m_data = nullptr;
    ImageDataType m_imageDataType;
};

struct MeshData2D {
    std::vector<Vertex2D> vertices;
    std::vector<uint32_t> indices;
};

struct Resolutions {
    glm::ivec2 gBuffer;
    glm::ivec2 finalImage;
    glm::ivec2 ui;
};

// INDIRECT RENDERING
struct DrawIndexedIndirectCommand {
    uint32_t indexCount = 0;
    uint32_t instanceCount = 0;
    uint32_t firstIndex = 0;
    int32_t  baseVertex = 0;
    uint32_t baseInstance = 0;
};
struct DrawArraysIndirectCommand {
    uint32_t vertexCount = 0;
    uint32_t instanceCount = 0;
    uint32_t firstVertex = 0;
    uint32_t baseInstance = 0;
};
struct DrawCommands {
    std::vector<DrawIndexedIndirectCommand> perViewport[4]; // maybe using splitscreen on future
};
struct DrawCommandsSet {
    DrawCommands geometry;
    DrawCommands geometryBlended;
    DrawCommands geometryAlphaDiscarded;

    DrawCommands skinnedGeometry;
};

struct FileInfo {
    std::string path;
    std::string name;
    std::string ext;
    std::string dir;
    std::string GetFileNameWithExtension() {
        if (ext.length() > 0) {
            return name + "." + ext;
        }
        else {
            return name;
        }
    }
};

// Bake stuff
struct QueuedTextureBake {
    void* texture = nullptr;
    int jobID = 0;
    int width = 0;
    int height = 0;
    int format = 0;
    int internalFormat = 0;
    int mipmapLevel = 0;
    int dataSize = 0;
    const void* data = nullptr;
    bool inProgress = false;
};

struct MeshData {
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    glm::vec3 aabbMin = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 aabbMax = glm::vec3(-std::numeric_limits<float>::max());
    uint32_t vertexCount;
    uint32_t indexCount;
};

struct ModelData {
    std::string name;
    uint32_t meshCount;
    uint64_t timestamp;
    std::vector<MeshData> meshes;
    glm::vec3 aabbMin = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 aabbMax = glm::vec3(-std::numeric_limits<float>::max());
};

struct BlitRect {
    int32_t x0 = 0;
    int32_t y0 = 0;
    int32_t x1 = 0;
    int32_t y1 = 0;
};
