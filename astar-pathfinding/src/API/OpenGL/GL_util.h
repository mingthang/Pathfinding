#pragma once

#include <glad/glad.h>
#include <string>
#include <Common/EngineEnums.h>

namespace OpenGLUtil {
    GLint GetFormatFromChannelCount(int channelCount);
    GLint GetInternalFormatFromChannelCount(int channelCount);

    // Engine Enums -> OpenGL translation
    GLint TextureWrapModeToGLEnum(TextureWrapMode wrapMode);
    GLint TextureFilterToGLEnum(TextureFilter filter);
    GLenum GLInternalFormatToGLType(GLenum internalFormat);
    GLenum GLInternalFormatToGLFormat(GLenum internalFormat);
}
