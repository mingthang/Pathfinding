#include "Texture.h"
#include <AssetManager/AssetManager.h>
#include <Tools/ImageTools.h>
#include <iostream>
#include <stb_image/stb_image.h>

void Texture::Load() {
	// Load texture data from disk
	if (m_imageDataType == ImageDataType::UNCOMPRESSED) {
		// Read data from image file by using stb_image, then return list of TextureData
		m_textureDataLevels = { ImageTools::LoadUncompressedTextureData(m_fileInfo.path) };
	}
	else if (m_imageDataType == ImageDataType::COMPRESSED) {
		// Not supported yet
	}
	else if (m_imageDataType == ImageDataType::EXR) {
		// Not supported yet
	}

	m_loadingState = LoadingState::LOADING_COMPLETE;
	
	m_mipmapLevelCount = 1 + static_cast<int>(std::log2(std::max(GetWidth(0), GetHeight(0))));

    m_textureDataLevelBakeStates.resize(m_textureDataLevels.size(), BakeState::AWAITING_BAKE);
}

void Texture::FreeCPUMemory() {
    for (TextureData& textureData : m_textureDataLevels) {
        stbi_image_free(textureData.m_data);
        textureData.m_data = nullptr;
    }
}

void Texture::RequestMipmaps() {
    m_mipmapsRequested = true;
}

const bool Texture::MipmapsAreRequested() {
    return m_mipmapsRequested;
}


void Texture::SetLoadingState(LoadingState loadingState) {
    m_loadingState = loadingState;
}

void Texture::SetFileInfo(FileInfo fileInfo) {
    m_fileInfo = fileInfo;
}

void Texture::SetImageDataType(ImageDataType imageDataType) {
    m_imageDataType = imageDataType;
}

void Texture::SetTextureWrapMode(TextureWrapMode wrapMode) {
    m_wrapMode = wrapMode;
}
void Texture::SetMinFilter(TextureFilter minFilter) {
    m_minFilter = minFilter;
}

void Texture::SetMagFilter(TextureFilter maxFilter) {
    m_magFilter = maxFilter;
}

void Texture::SetTextureDataLevelBakeState(int index, BakeState state) {
    if (index >= 0 && m_textureDataLevelBakeStates.size() && index < m_textureDataLevelBakeStates.size()) {
        m_textureDataLevelBakeStates[index] = state;
    }
    else {
        std::cout << "Texture::SetTextureDataLevelBakeState(int index, BakeState state) failed. Index '" << index << "' out of range of size " << m_textureDataLevelBakeStates.size() << "\n";
    }
}

void Texture::CheckForBakeCompletion() {
    if (m_bakeCompleted) {
        return;
    }
    else {
        m_bakeCompleted = true;
        for (BakeState& state : m_textureDataLevelBakeStates) {
            if (state != BakeState::BAKE_COMPLETE) {
                m_bakeCompleted = false;
                return;
            }
        }

        // Bake is completed!
        //std::cout << GetFilePath() << " baked" << std::endl;
        AssetManager::AddItemToLoadLog(GetFilePath());
    }
}

const bool Texture::BakeComplete() {
    return m_bakeCompleted;
}

const int Texture::GetWidth(int mipmapLevel) {
    if (mipmapLevel >= 0 && mipmapLevel < m_textureDataLevels.size()) {
        return m_textureDataLevels[mipmapLevel].m_width;
    }
    else {
        std::cout << "Texture::GetWidth(int mipmapLevel) failed. mipmapLevel '" << mipmapLevel << "' out of range of size " << m_textureDataLevels.size() << "\n";
        return 0;
    }
}

OpenGLTexture& Texture::GetGLTexture() {
    return m_glTexture;
}

const int Texture::GetHeight(int mipmapLevel) {
    if (mipmapLevel >= 0 && mipmapLevel < m_textureDataLevels.size()) {
        return m_textureDataLevels[mipmapLevel].m_height;
    }
    else {
        std::cout << "Texture::GetHeight(int mipmapLevel) failed. mipmapLevel '" << mipmapLevel << "' out of range of size " << m_textureDataLevels.size() << "\n";
        return 0;
    }
}

const int Texture::GetFormat() {
    if (!m_textureDataLevels.empty()) {
        return m_textureDataLevels[0].m_format;
    }
    else {
        std::cout << "Texture::GetFormat() FAILED: m_textureData is empty." << "\n";
        return 0;
    }
}

const int Texture::GetInternalFormat() {
    if (!m_textureDataLevels.empty()) {
        return m_textureDataLevels[0].m_internalFormat;
    }
    else {
        std::cout << "Texture::GetInternalFormat() FAILED: m_textureData is empty." << "\n";
        return 0;
    }
}

const int Texture::GetMipmapLevelCount() {
    return m_mipmapLevelCount;
}

const void* Texture::GetData(int mipmapLevel) {
    if (mipmapLevel >= 0 && mipmapLevel < m_textureDataLevels.size()) {
        return m_textureDataLevels[mipmapLevel].m_data;
    }
    else {
        std::cout << "Texture::GetData(int mipmapLevel) FAILED: mipmapLevel " << mipmapLevel << " out of range of size " << m_textureDataLevels.size() << "\n";
        return nullptr;
    }
}

const int Texture::GetDataSize(int mipmapLevel) {
    if (mipmapLevel >= 0 && mipmapLevel < m_textureDataLevels.size()) {
        return m_textureDataLevels[mipmapLevel].m_dataSize;
    }
    else {
        std::cout << "Texture::GetDataSize(int mipmapLevel) FAILED: mipmapLevel " << mipmapLevel << " out of range of size " << m_textureDataLevels.size() << "\n";
        return 0;
    }
}

const int Texture::GetChannelCount() {
    if (!m_textureDataLevels.empty()) {
        return m_textureDataLevels[0].m_channelCount;
    }
    else {
        std::cout << "Texture::GetChannelCount() FAILED: m_textureData is empty." << "\n";
        return 0;
    }
}

const int Texture::GetTextureDataCount() {
    return m_textureDataLevels.size();
}

const std::string& Texture::GetFileName() {
    return m_fileInfo.name;
}

const std::string& Texture::GetFilePath() {
    return m_fileInfo.path;
}

const FileInfo Texture::GetFileInfo() {
    return m_fileInfo;
}

const ImageDataType Texture::GetImageDataType() {
    return m_imageDataType;
}

const TextureWrapMode Texture::GetTextureWrapMode() {
    return m_wrapMode;
}

const TextureFilter Texture::GetMinFilter() {
    return m_minFilter;
}

const TextureFilter Texture::GetMagFilter() {
    return m_magFilter;
}

const LoadingState Texture::GetLoadingState() {
    return m_loadingState;
}
