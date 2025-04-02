#include <API/OpenGL/Renderer/GL_renderer.h>
#include <Common/EngineTypes.h>
#include <Renderer/RenderDataManager.h>
#include <Core/GridMap/GridMap.h>
#include <API/OpenGL/Types/GL_shader.h>
#include <API/OpenGL/Types/GL_texture.h>
#include <AssetManager/AssetManager.h>
#include <UI/Mesh2D.h>
#include <UI/UIBackEnd.h>
#include <vector>
#include <API/OpenGL/GL_backEnd.h>
#include <BackEnd/BackEnd.h>

namespace OpenGLRenderer {

	void GridPass() {
        OpenGLShader* shader = GetShader("Tile");
        OpenGLFrameBuffer* gBuffer = GetFrameBuffer("GBuffer");
		if (!gBuffer) return;
		if (!shader) return;

        gBuffer->Bind();
        // Here we can do multiple viewports, but for now only one
        gBuffer->SetViewport();
        gBuffer->ClearAttachment("BaseColor", 1.0f, 0.0f, 0.0f, 1.0f);
        gBuffer->DrawBuffers({ "BaseColor" });

        shader->Use();

        // GL State
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(OpenGLBackEnd::GetVertexDataVAO());

        std::vector<RenderItem2D> gridRenderItems = GridMap::CreateRenderItems2D();

        for (const RenderItem2D& renderItem : gridRenderItems) {
            Mesh* mesh = AssetManager::GetMeshByIndex(AssetManager::GetMeshIndexByName("Quad"));
            if (!mesh) {
                continue;
            }
            shader->SetMat4("u_modelMatrix", renderItem.modelMatrix);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, AssetManager::GetTextureByIndex(renderItem.textureIndex)->GetGLTexture().GetHandle());

            glDrawElementsBaseVertex(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * mesh->baseIndex), mesh->baseVertex);
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }
}