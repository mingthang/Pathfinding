#pragma once

#include <Common/EngineTypes.h>
#include <vector>
#include <glad/glad.h>

struct OpenGLMesh2D {
private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    GLsizei m_indexCount = 0;
    GLsizei m_vertexBufferSize = 0;
    GLsizei m_indexBufferSize = 0;

public: 
    void UpdateVertexBuffer(std::vector<Vertex2D>& vertices, std::vector<uint32_t>& indices) {

    }
};
