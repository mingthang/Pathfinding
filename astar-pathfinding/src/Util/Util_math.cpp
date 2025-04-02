#include "Util.h"

namespace Util {

    float MapRange(float inValue, float minInRange, float maxInRange, float minOutRange, float maxOutRange) {
        float x = (inValue - minInRange) / (maxInRange - minInRange);
        return minOutRange + (maxOutRange - minOutRange) * x;
    }

    glm::vec2 ScreenToNDC(glm::vec2 screenPos, glm::vec2 viewportSize) {
        float ndcX = 2.0f * (static_cast<float>(screenPos.x) / viewportSize.x) - 1.0f;
        float ndcY = 1.0f - 2.0f * (static_cast<float>(screenPos.y) / viewportSize.y);
        return glm::vec2(ndcX, ndcY);
    }

}