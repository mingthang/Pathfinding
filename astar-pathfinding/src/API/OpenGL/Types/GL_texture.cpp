#include "GL_texture.h"
#include <API/OpenGL/GL_util.h>
#include <BackEnd/BackEnd.h>
#include <glad/glad.h>

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

void OpenGLTexture::MakeBindlessTextureResident() {
	if (BackEnd::RenderDocFound()) return;

	if (m_bindlessID == 0) {
		m_bindlessID = glGetTextureHandleARB(m_handle);
	}
	glMakeTextureHandleResidentARB(m_bindlessID);
}

void OpenGLTexture::MakeBindlessTextureNonResident() {
	if (BackEnd::RenderDocFound()) return;

	if (m_bindlessID != 0) {
		glMakeTextureHandleNonResidentARB(m_bindlessID);
		m_bindlessID = 0;
	}
}

void OpenGLTexture::SetWrapMode(TextureWrapMode wrapMode) {
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, OpenGLUtil::TextureWrapModeToGLEnum(wrapMode));
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, OpenGLUtil::TextureWrapModeToGLEnum(wrapMode));
}

void OpenGLTexture::SetMinFilter(TextureFilter filter) {
	glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, OpenGLUtil::TextureFilterToGLEnum(filter));
}

void OpenGLTexture::SetMagFilter(TextureFilter filter) {
	glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, OpenGLUtil::TextureFilterToGLEnum(filter));
}

GLuint& OpenGLTexture::GetHandle() {
	return m_handle;
}

int OpenGLTexture::GetWidth() {
	return m_width;
}

int OpenGLTexture::GetHeight() {
	return m_height;
}

int OpenGLTexture::GetChannelCount() {
	return m_channelCount;
}

int OpenGLTexture::GetDataSize() {
	return m_dataSize;
}

void* OpenGLTexture::GetData() {
	return m_data;
}

GLint OpenGLTexture::GetFormat() {
	return m_format;
}
GLint OpenGLTexture::GetInternalFormat() {
	return m_internalFormat;
}

GLint OpenGLTexture::GetMipmapLevelCount() {
	return m_mipmapLevelCount;
}