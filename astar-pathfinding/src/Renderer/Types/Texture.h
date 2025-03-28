#pragma once

#include <API/OpenGL/Types/GL_texture.h>

struct Texture {
public:
	Texture() = default;
	void Load();

private:
	OpenGLTexture m_glTexture;
};
