#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAX_RENDER_OBJECTS_2D 4096

struct ViewportInfo {
    int width = 0;
    int height = 0;
    int xOffset = 0;
    int yOffset = 0;
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