#pragma once

#include <API/OpenGL/Types/GL_mesh2D.h>

struct Mesh2D {
    OpenGLMesh2D& GetGLMesh2D();

private:
    OpenGLMesh2D glMesh2D;
};
