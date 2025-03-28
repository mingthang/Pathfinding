#include "GL_texture.h"

void OpenGLTexture::AllocateMemory(int width, int height, int format, int internalFormat, int mipmapLevelCount) {
	if (m_memoryAllocated)
		return;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
	// immutable-format texture	
	glTextureStorage2D(m_handle, mipmapLevelCount, internalFormat, width, height);
	m_width = width;
	m_height = height;
	m_mipmapLevelCount = mipmapLevelCount;
	glBindTexture(GL_TEXTURE_2D, m_handle);
	int mipmapWidth = width;
	int mipmapHeight = height;
	for (int i = 0; i < mipmapLevelCount; ++i) {
		if(m_imageDataType == ImageDataType::UNCOMPRESSED)
			glTextureSubImage2D(m_handle, i, 0, 0, mipmapWidth, mipmapHeight, format, GL_UNSIGNED_BYTE, nullptr);
		if(m_imageDataType == ImageDataType::COMPRESSED)
			glCompressedTextureSubImage2D(m_handle, i, 0, 0, mipmapWidth, mipmapHeight, internalFormat, 0, nullptr);
		if (m_imageDataType == ImageDataType::EXR)
			glTextureSubImage2D(m_handle, i, 0, 0, mipmapWidth, mipmapHeight, GL_RGBA, GL_FLOAT, nullptr);
		mipmapWidth = std::max(1, mipmapWidth / 2);
		mipmapHeight = std::max(1, mipmapHeight / 2);
	}

	m_memoryAllocated = true;
}