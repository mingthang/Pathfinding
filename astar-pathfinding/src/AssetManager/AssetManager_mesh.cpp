#include "AssetManager.h"
#include <iostream>

namespace AssetManager {
    int g_nextVertexInsert = 0;
    int g_nextIndexInsert = 0;

    int CreateMesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, glm::vec3 aabbMin, glm::vec3 aabbMax) {
        std::vector<Mesh>& meshes = GetMeshes();
        std::vector<Vertex>& allVertices = GetVertices();
        std::vector<uint32_t>& allIndices = GetIndices();

        Mesh& mesh = meshes.emplace_back();
        mesh.baseVertex = g_nextVertexInsert;
        mesh.baseIndex = g_nextIndexInsert;
        mesh.vertexCount = (uint32_t)vertices.size();
        mesh.indexCount = (uint32_t)indices.size();
        mesh.SetName(name);
        mesh.aabbMin = aabbMin;
        mesh.aabbMax = aabbMax;
        mesh.extents = aabbMax - aabbMin;
        mesh.boundingSphereRadius = std::max(mesh.extents.x, std::max(mesh.extents.y, mesh.extents.z)) * 0.5f;

        allVertices.reserve(allVertices.size() + vertices.size());
        allVertices.insert(std::end(allVertices), std::begin(vertices), std::end(vertices));
        allIndices.reserve(allIndices.size() + indices.size());
        allIndices.insert(std::end(allIndices), std::begin(indices), std::end(indices));
        g_nextVertexInsert += mesh.vertexCount;
        g_nextIndexInsert += mesh.indexCount;

        return meshes.size() - 1;
    }

    int GetMeshIndexByName(const std::string& name) {
        std::vector<Mesh>& meshes = GetMeshes();
        for (int i = 0; i < meshes.size(); i++) {
            if (meshes[i].GetName() == name)
                return i;
        }
        std::cout << "AssetManager::GetMeshIndexByName() failed because '" << name << "' does not exist\n";
        return -1;
    }

    Mesh* GetMeshByName(const std::string& name) {
        std::vector<Mesh>& meshes = GetMeshes();
        for (int i = 0; i < meshes.size(); i++) {
            if (meshes[i].GetName() == name)
                return &meshes[i];
        }
        std::cout << "AssetManager::GetMeshByName() failed because '" << name << "' does not exist\n";
        return nullptr;
    }

    Mesh* GetMeshByIndex(int index) {
        std::vector<Mesh>& meshes = GetMeshes();
        if (index >= 0 && index < meshes.size()) {
            return &meshes[index];
        }
        else {
            std::cout << "AssetManager::GetMeshByIndex() failed because index '" << index << "' is out of range. Size is " << meshes.size() << "!\n";
            return nullptr;
        }
    }

    std::vector<Vertex> GetMeshVertices(Mesh* mesh) {
        if (!mesh) {
            std::cout << "AssetManager::GetMeshVertices() failed: mesh was nullptr\n";
            return std::vector<Vertex>();
        }

        std::vector<Vertex>& vertices = AssetManager::GetVertices();
        std::vector<uint32_t>& indices = AssetManager::GetIndices();

        std::vector<Vertex> result;
        result.reserve(mesh->vertexCount);

        for (int i = mesh->baseIndex; i < mesh->baseIndex + mesh->indexCount; i++) {
            uint32_t index = indices[i];
            const Vertex& vertex = vertices[index + mesh->baseVertex];
            result.push_back(vertex);
        }

        return result;
    }
}