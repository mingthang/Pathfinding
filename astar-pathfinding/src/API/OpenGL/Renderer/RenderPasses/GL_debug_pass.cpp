#include <API/OpenGL/Renderer/GL_renderer.h>
#include <API/OpenGL/Types/GL_debug_mesh.h>
#include <API/OpenGL/Types/GL_shader.h>

namespace OpenGLRenderer {

    inline std::vector<DebugVertex> g_points;
    inline std::vector<DebugVertex> g_lines;
    inline std::vector<DebugVertex> g_pointsDepthAware;
    inline std::vector<DebugVertex> g_linesDepthAware;

	OpenGLDebugMesh g_debugMeshPoints;
	OpenGLDebugMesh g_debugMeshLines;
	OpenGLDebugMesh g_debugMeshDepthAwarePoints;
	OpenGLDebugMesh g_debugMeshDepthAwareLines;

	void DebugPass() {
		OpenGLShader* shader = GetShader("DebugVertex");
		OpenGLFrameBuffer* gBuffer = GetFrameBuffer("GBuffer");
		if (!gBuffer) return;
		if (!shader) return;

        gBuffer->Bind();
        gBuffer->DrawBuffer("BaseColor");

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
        glPointSize(8.0f);

        shader->Use();
        // Here we can do multiple viewports, but for now only one
        gBuffer->SetViewport();
        //shader->SetMat4("u_projectionView", viewportData[0].projectionView);

        glDisable(GL_DEPTH_TEST);
        if (g_debugMeshPoints.GetVertexCount() > 0) {
            glBindVertexArray(g_debugMeshPoints.GetVAO());
            glDrawArrays(GL_POINTS, 0, g_debugMeshPoints.GetVertexCount());
        }
        if (g_debugMeshLines.GetVertexCount() > 0) {
            glBindVertexArray(g_debugMeshLines.GetVAO());
            glDrawArrays(GL_LINES, 0, g_debugMeshLines.GetVertexCount());
        }
        // Depth Aware
        glEnable(GL_DEPTH_TEST);
        if (g_debugMeshDepthAwarePoints.GetVertexCount() > 0) {
            glBindVertexArray(g_debugMeshPoints.GetVAO());
            glDrawArrays(GL_POINTS, 0, g_debugMeshPoints.GetVertexCount());
        }
        if (g_debugMeshDepthAwareLines.GetVertexCount() > 0) {
            glBindVertexArray(g_debugMeshDepthAwareLines.GetVAO());
            glDrawArrays(GL_LINES, 0, g_debugMeshDepthAwareLines.GetVertexCount());
        }
	}

    void UpdateDebugMesh() {
        g_debugMeshPoints.UpdateVertexData(g_points);
        g_debugMeshDepthAwarePoints.UpdateVertexData(g_pointsDepthAware);
        g_debugMeshLines.UpdateVertexData(g_lines);
        g_debugMeshDepthAwareLines.UpdateVertexData(g_linesDepthAware);

        g_points.clear();
        g_lines.clear();
        g_pointsDepthAware.clear();
        g_linesDepthAware.clear();
    }

    void DrawPoint(glm::vec3 position, glm::vec3 color, bool obeyDepth, int exclusiveViewportIndex) {
        if (obeyDepth) {
            g_pointsDepthAware.push_back(DebugVertex(position, color, glm::ivec2(0, 0), exclusiveViewportIndex));
        }
        else {
            g_points.push_back(DebugVertex(position, color, glm::ivec2(0, 0), exclusiveViewportIndex));
        }
    }

    void DrawLine(glm::vec3 begin, glm::vec3 end, glm::vec3 color, bool obeyDepth, int exclusiveViewportIndex) {
        DebugVertex v0 = DebugVertex(begin, color, glm::ivec2(0, 0), exclusiveViewportIndex);
        DebugVertex v1 = DebugVertex(end, color, glm::ivec2(0, 0), exclusiveViewportIndex);
        if (obeyDepth) {
            g_linesDepthAware.push_back(v0);
            g_linesDepthAware.push_back(v1);
        }
        else {
            g_lines.push_back(v0);
            g_lines.push_back(v1);
        }
    }
}