#include "Assets/MeshManager.h"
#include <spdlog/spdlog.h>

const Mesh* MeshManager::GetMesh(const std::string& meshName) const
{
    auto it = m_MeshMap.find(meshName);
    if (it != m_MeshMap.end()) return &it->second;

    spdlog::warn("Mesh '{}' not found in cache.", meshName);
    return nullptr;
}

const Mesh* MeshManager::LoadMesh(const std::string& meshName)
{
    auto it = m_MeshMap.find(meshName);
    if (it != m_MeshMap.end())
    {
        spdlog::info("Mesh '{}' loaded from cache.", meshName);
        return &it->second;
    }

    std::string fullPath = m_MeshDirectory + "/" + meshName;

    uint32_t importOptions = aiProcess_Triangulate
        | aiProcess_GenNormals
        | aiProcess_FlipUVs;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile
	(
        fullPath,
        importOptions
    );

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        spdlog::error("Assimp failed to load mesh '{}': {}", meshName, importer.GetErrorString());
        return nullptr;
    }

    aiMesh* mesh = scene->mMeshes[0];

    VertexBufferLayout layout;
    std::vector<Vertex> vertices = LoadVertices(mesh, layout);
    if (vertices.empty())
    {
        spdlog::error("Failed to load vertices for mesh '{}'", meshName);
        return nullptr;
    }

    VertexBuffer vertexBuffer(vertices.data(), mesh->mNumVertices * layout.GetStride());

    std::vector<uint32_t> indices = LoadIndices(mesh);
    if (indices.empty())
    {
        spdlog::error("Failed to load indices for mesh '{}'", meshName);
        return nullptr;
    }

    IndexBuffer indexBuffer(indices.data(), mesh->mNumFaces * 3);

    VertexArray vao;
    vao.Bind();
    vao.AddBuffer(std::move(vertexBuffer), layout);
    indexBuffer.Bind();
    vao.Unbind();

    Mesh newMesh(std::move(vao), std::move(indexBuffer));

    auto result = m_MeshMap.emplace(meshName, std::move(newMesh));
    return &result.first->second;
}

std::vector<MeshManager::Vertex> MeshManager::LoadVertices(const aiMesh* mesh, VertexBufferLayout& layout)
{
    std::vector<Vertex> vertices(mesh->mNumVertices);

    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        if (mesh->HasPositions())
        {
            vertices[i].Position[0] = mesh->mVertices[i].x;
            vertices[i].Position[1] = mesh->mVertices[i].y;
            vertices[i].Position[2] = mesh->mVertices[i].z;
            if (i == 0) layout.Push<float>(3);
        }
        if (mesh->HasNormals())
        {
            vertices[i].Normal[0] = mesh->mNormals[i].x;
            vertices[i].Normal[1] = mesh->mNormals[i].y;
            vertices[i].Normal[2] = mesh->mNormals[i].z;
            if (i == 0) layout.Push<float>(3);
        }
        if (mesh->HasTextureCoords(0))
        {
            vertices[i].Texture[0] = mesh->mTextureCoords[0][i].x;
            vertices[i].Texture[1] = mesh->mTextureCoords[0][i].y;
            if (i == 0) layout.Push<float>(2);
        }
        if (mesh->HasTangentsAndBitangents())
        {
            vertices[i].Tangent[0] = mesh->mTangents[i].x;
            vertices[i].Tangent[1] = mesh->mTangents[i].y;
            vertices[i].Tangent[2] = mesh->mTangents[i].z;
            if (i == 0) layout.Push<float>(3);
        }
    }

    return vertices;
}

std::vector<uint32_t> MeshManager::LoadIndices(aiMesh* mesh)
{
    std::vector<uint32_t> indices;

    if (mesh->HasFaces())
    {
        indices.resize(mesh->mNumFaces * 3);

        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            indices[i * 3] = mesh->mFaces[i].mIndices[0];
            indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
            indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
        }
    }

    return indices;
}

