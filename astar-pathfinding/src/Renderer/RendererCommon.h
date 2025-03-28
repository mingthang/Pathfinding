#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAX_RENDER_OBJECTS_2D 4096

enum Alignment {
    CENTERED,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

struct ViewportInfo {
    int width = 0;
    int height = 0;
    int xOffset = 0;
    int yOffset = 0;
};

struct RenderItem2D {
    glm::mat4 modelMatrix = glm::mat4(1);
    float colorTintR;
    float colorTintG;
    float colorTintB;
    int textureIndex;
};

struct CameraData {

    glm::mat4 projection = glm::mat4(1);
    glm::mat4 projectionInverse = glm::mat4(1);
    glm::mat4 view = glm::mat4(1);
    glm::mat4 viewInverse = glm::mat4(1);

    float viewportWidth = 0;
    float viewportHeight = 0;
    float viewportOffsetX = 0;
    float viewportOffsetY = 0;

    float clipSpaceXMin;
    float clipSpaceXMax;
    float clipSpaceYMin;
    float clipSpaceYMax;

    float contrast;
    float colorMultiplierR;
    float colorMultiplierG;
    float colorMultiplierB;
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
    glm::vec3 position = glm::vec3(0);
    glm::vec3 normal = glm::vec3(0);
    glm::vec2 uv = glm::vec2(0);
    glm::vec3 tangent = glm::vec3(0);

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal && uv == other.uv;
    }
};
