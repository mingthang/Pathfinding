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

    GLuint g_gridModelVBO = 0;
    GLuint g_gridColorVBO = 0;

    void InitInstanceGridBuffers() {
        glGenBuffers(1, &g_gridModelVBO);
        glGenBuffers(1, &g_gridColorVBO);

        Mesh* mesh = AssetManager::GetMeshByIndex(AssetManager::GetMeshIndexByName("Quad"));
		if (!mesh) {
			return;
		}

        glBindVertexArray(OpenGLBackEnd::GetVertexDataVAO());

        // Model matrix
        glBindBuffer(GL_ARRAY_BUFFER, g_gridModelVBO);
        for (int i = 0; i < 4; i++) {
            glEnableVertexAttribArray(4 + i);
            glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
            glVertexAttribDivisor(4 + i, 1);
        }

        // Color
        glBindBuffer(GL_ARRAY_BUFFER, g_gridColorVBO);
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
        glVertexAttribDivisor(8, 1);

        glBindVertexArray(0);
    }

	void GridPass() {
        OpenGLShader* shader = GetShader("Tile");
        OpenGLFrameBuffer* gBuffer = GetFrameBuffer("GBuffer");
		if (!gBuffer || !shader) return;

        gBuffer->Bind();
        // Here we can do multiple viewports, but for now only one
        gBuffer->SetViewport();
        gBuffer->ClearAttachment("BaseColor", 0.0f, 0.0f, 0.0f, 1.0f);
        gBuffer->DrawBuffers({ "BaseColor" });

        shader->Use();

        // GL State
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(OpenGLBackEnd::GetVertexDataVAO());

        const std::vector<RenderItem2D>& gridLayer = GridMap::GetGridLayer();
        if (!gridLayer.empty()) {
            std::vector<glm::mat4> modelMatrices;
            std::vector<glm::vec4> colors;
			int selectorIndex = GridMap::GetMouseCellX() * GridMap::GetMapHeight() + GridMap::GetMouseCellY();
            int startIndex = GridMap::GetStartX() * GridMap::GetMapHeight() + GridMap::GetStartY();
            int targetIndex = GridMap::GetTargetX() * GridMap::GetMapHeight() + GridMap::GetTargetY();
            for (const auto& item : gridLayer) {
                modelMatrices.push_back(item.modelMatrix);
                colors.emplace_back(item.colorTintR, item.colorTintG, item.colorTintB, 1.0f);
            }
            colors[selectorIndex] = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
            colors[startIndex] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
            colors[targetIndex] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

            // Upload model matrices
            glBindBuffer(GL_ARRAY_BUFFER, g_gridModelVBO);
            glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data(), GL_DYNAMIC_DRAW);

            // Upload colors
            glBindBuffer(GL_ARRAY_BUFFER, g_gridColorVBO);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_DYNAMIC_DRAW);

            // Bind texture
            Texture* tex = AssetManager::GetTextureByIndex(gridLayer[0].textureIndex);
            if (tex) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex->GetGLTexture().GetHandle());
            }

            // Draw
            Mesh* mesh = AssetManager::GetMeshByIndex(AssetManager::GetMeshIndexByName("Quad"));
            if (mesh) {
                glDrawElementsInstancedBaseVertex(
                    GL_TRIANGLES,
                    mesh->indexCount,
                    GL_UNSIGNED_INT,
                    (void*)(sizeof(unsigned int) * mesh->baseIndex),
                    static_cast<GLsizei>(gridLayer.size()),
                    mesh->baseVertex
                );
            }
        }

        //const std::vector<RenderItem2D>& selectorLayer = GridMap::GetSelectorLayer();
        //for (const auto& item : selectorLayer) {
        //    Texture* tex = AssetManager::GetTextureByIndex(item.textureIndex);
        //    if (!tex)
        //        continue;

        //    glActiveTexture(GL_TEXTURE0);
        //    glBindTexture(GL_TEXTURE_2D, tex->GetGLTexture().GetHandle());

        //    Mesh* mesh = AssetManager::GetMeshByIndex(AssetManager::GetMeshIndexByName("Quad"));
        //    if (!mesh)
        //        continue;

        //    shader->SetMat4("u_modelMatrix", item.modelMatrix);
        //    shader->SetVec4("u_color", glm::vec4(item.colorTintR, item.colorTintG, item.colorTintB, 1.0f));

        //    glDrawElementsBaseVertex(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT,
        //        (void*)(sizeof(unsigned int) * mesh->baseIndex), mesh->baseVertex);
        //}

 //       std::unordered_map<int, std::vector<RenderItem2D>> batches;
 //       for (const auto& item : renderItems) {
 //           batches[item.textureIndex].push_back(item);
 //       }

 //       for (const auto& batch : batches) {
 //           Texture* tex = AssetManager::GetTextureByIndex(batch.first);
 //           if (!tex)
 //               continue;

 //           glActiveTexture(GL_TEXTURE0);
 //           glBindTexture(GL_TEXTURE_2D, tex->GetGLTexture().GetHandle());

 //           Mesh* mesh = AssetManager::GetMeshByIndex(AssetManager::GetMeshIndexByName("Quad"));
 //           if (!mesh)
 //               continue;

 //           // Prepare instance data
 //           std::vector<glm::mat4> modelMatrices;
 //           std::vector<glm::vec4> colors;

 //           for (const auto& renderItem : batch.second) {
 //               modelMatrices.push_back(renderItem.modelMatrix);
 //               colors.push_back(glm::vec4(renderItem.colorTintR, renderItem.colorTintG, renderItem.colorTintB, 1.0f));
 //           }

 //           // Set uniform for model matrices and colors
 //           glUniformMatrix4fv(glGetUniformLocation(shader->GetHandle(), "u_modelMatrices"), modelMatrices.size(), GL_TRUE, &modelMatrices[0][0][0]);
 //           glUniform4fv(glGetUniformLocation(shader->GetHandle(), "u_colors"), colors.size(), &colors[0][0]);

 //           // Draw instanced
 //           glDrawElementsInstanced(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * mesh->baseIndex), modelMatrices.size());

 ///*           for (const auto& renderItem : batch.second) {
 //               shader->SetMat4("u_modelMatrix", renderItem.modelMatrix);
 //               shader->SetVec4("u_color", glm::vec4(renderItem.colorTintR, renderItem.colorTintG, renderItem.colorTintB, 1.0f));
 //               glDrawElementsBaseVertex(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT,
 //                   (void*)(sizeof(unsigned int) * mesh->baseIndex), mesh->baseVertex);
 //           }*/
 //       }

        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }
}