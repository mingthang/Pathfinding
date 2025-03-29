#include "ImageTools.h"
#include <stb_image/stb_image.h>
#include <API/OpenGL/GL_util.h>

namespace ImageTools {
	TextureData LoadUncompressedTextureData(const std::string& filePath) {
		stbi_set_flip_vertically_on_load(false);
		TextureData textureData;
		uint8_t* imageData = stbi_load(filePath.data(), &textureData.m_width, &textureData.m_height, &textureData.m_channelCount, 0);
		textureData.m_imageDataType = ImageDataType::UNCOMPRESSED;

		if (textureData.m_channelCount == 3) {
			size_t newSize = textureData.m_width * textureData.m_height * 4;
			uint8_t* rgbaData = new uint8_t[newSize];
			for (size_t i = 0, j = 0; i < newSize; i += 4, j += 3) {
				rgbaData[i] = imageData[j];				// R
				rgbaData[i + 1] = imageData[j + 1];		// G
				rgbaData[i + 2] = imageData[j + 2];		// B
				rgbaData[i + 3] = 255;					// A
			}
			stbi_image_free(imageData);
			textureData.m_data = rgbaData;
			textureData.m_channelCount = 4;
		}
		else {
			textureData.m_data = imageData;
		}

		// If mipmaps are requested allocate space for them
		if (textureData.m_channelCount == 4 && textureData.m_width != textureData.m_height) {
			textureData.m_dataSize = textureData.m_width * textureData.m_height * 4;
		}
		else {
			textureData.m_dataSize = textureData.m_width * textureData.m_height * textureData.m_channelCount;
		}

		textureData.m_format = OpenGLUtil::GetFormatFromChannelCount(textureData.m_channelCount);
		textureData.m_internalFormat = OpenGLUtil::GetInternalFormatFromChannelCount(textureData.m_channelCount);

		return textureData;
	}
}