#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EngineEnums.h"

struct RenderItem2D {
    glm::mat4 modelMatrix = glm::mat4(1);
    float colorTintR = 1.0f;
    float colorTintG = 1.0f;
    float colorTintB = 1.0f;
    int textureIndex = -1;
    int baseVertex = 0;
    int baseIndex = 0;
};

struct Vertex2D {
    glm::vec2 position;
    glm::vec2 uv;
    glm::vec4 color;
    int textureIndex;
};

struct MeshData2D {
    std::vector<Vertex2D> vertices;
    std::vector<uint32_t> indices;
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
