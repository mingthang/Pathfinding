#pragma once

#include <API/OpenGL/Types/GL_texture.h>
#include <Common/EngineEnums.h>
#include <vector>

struct Texture {
public:
	Texture() = default;
	void Load();
	void RequestMipmaps();
	void FreeCPUMemory();

	void RequestMipmaps();
	const bool MipmapsAreRequested();

	void SetLoadingState(LoadingState loadingState);
	void SetFileInfo(FileInfo fileInfo);
	void SetImageDataType(ImageDataType imageDataType);
	void SetTextureWrapMode(TextureWrapMode wrapMode);
	void SetMinFilter(TextureFilter minFilter);
	void SetMagFilter(TextureFilter magFilter);
	void SetTextureDataLevelBakeState(int index, BakeState bakeState);

	OpenGLTexture& GetGLTexture();
	const int GetWidth(int mipmapLevel);
	const int GetHeight(int mipmapLevel);
	const int GetFormat();
	const int GetInternalFormat();
	const int GetMipmapLevelCount();
	const void* GetData(int mipmapLevel);
	const int GetDataSize(int mipmapLevel);
	const int GetChannelCount();
	const std::string& GetFileName();
	const std::string& GetFilePath();
	const FileInfo GetFileInfo();
	const ImageDataType GetImageDataType();
	const TextureWrapMode GetTextureWrapMode();
	const TextureFilter GetMinFilter();
	const TextureFilter GetMagFilter();
	const LoadingState GetLoadingState();
	const BakeState GetTextureDataLevelBakeState(int index);

private:
	OpenGLTexture m_glTexture;
	LoadingState m_loadingState = LoadingState::AWAITING_LOADING_FROM_DISK;
	ImageDataType m_imageDataType = ImageDataType::UNDEFINED;
	TextureWrapMode m_wrapMode = TextureWrapMode::UNDEFINED;
	TextureFilter m_minFilter = TextureFilter::NEAREST;
	TextureFilter m_magFilter = TextureFilter::NEAREST;
	FileInfo m_fileInfo;
	std::vector<TextureData> m_textureDataLevels;
	std::vector<BakeState> m_textureDataLevelBakeStates;
	int m_mipmapLevelCount = 0;
	bool m_mipmapsRequested = false;
	bool m_bakeCompleted = false;
};
