#include "Assets/MeshManager.h"
#include <spdlog/spdlog.h>

struct Vertex
{
    float Position[3];
    float Normal[3];
    float Texture[2];
    float Tangent[3];
};

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

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fullPath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        spdlog::error("Assimp failed to load mesh '{}': {}", meshName, importer.GetErrorString());
        return nullptr;
    }

    aiMesh* mesh = scene->mMeshes[0];

    VertexBufferLayout layout;
    std::unique_ptr<void, decltype(&free)> vertices(LoadVertices(mesh, layout), free);
    if (!vertices)
    {
        spdlog::error("Failed to load vertices for mesh '{}'", meshName);
        return nullptr;
    }

    VertexBuffer vertexBuffer(vertices.get(), mesh->mNumVertices * layout.GetStride());

    std::unique_ptr<void, decltype(&free)> indices(LoadIndices(mesh), free);
    if (!indices)
    {
        spdlog::error("Failed to load indices for mesh '{}'", meshName);
        return nullptr;
    }

    IndexBuffer indexBuffer(indices.get(), mesh->mNumFaces * 3);

    VertexArray vao;
    vao.Bind();
    vao.AddBuffer(std::move(vertexBuffer), layout);
    indexBuffer.Bind();
    vao.Unbind();

    Mesh newMesh(std::move(vao), std::move(indexBuffer));

    auto result = m_MeshMap.emplace(meshName, std::move(newMesh));
    return &result.first->second;
}

void* MeshManager::LoadVertices(const aiMesh* mesh, VertexBufferLayout& layout)
{
    Vertex* pVertices = new Vertex[mesh->mNumVertices];
    std::memset(pVertices, 0, sizeof(Vertex) * mesh->mNumVertices);

    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        if (mesh->HasPositions())
        {
            pVertices[i].Position[0] = mesh->mVertices[i].x;
            pVertices[i].Position[1] = mesh->mVertices[i].y;
            pVertices[i].Position[2] = mesh->mVertices[i].z;
            if (i == 0) layout.Push<float>(3);
        }
        if (mesh->HasNormals())
        {
            pVertices[i].Normal[0] = mesh->mNormals[i].x;
            pVertices[i].Normal[1] = mesh->mNormals[i].y;
            pVertices[i].Normal[2] = mesh->mNormals[i].z;
            if (i == 0) layout.Push<float>(3);
        }
        if (mesh->HasTextureCoords(0))
        {
            pVertices[i].Texture[0] = mesh->mTextureCoords[0][i].x;
            pVertices[i].Texture[1] = mesh->mTextureCoords[0][i].y;
            if (i == 0) layout.Push<float>(2);
        }
        if (mesh->HasTangentsAndBitangents())
        {
            pVertices[i].Tangent[0] = mesh->mTangents[i].x;
            pVertices[i].Tangent[1] = mesh->mTangents[i].y;
            pVertices[i].Tangent[2] = mesh->mTangents[i].z;
            if (i == 0) layout.Push<float>(3);
        }
    }

    return pVertices;
}

void* MeshManager::LoadIndices(aiMesh* mesh)
{
    uint32_t* pIndices = nullptr;

    if (mesh->HasFaces())
    {

        pIndices = new uint32_t[mesh->mNumFaces * 3];
        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            pIndices[i * 3] = mesh->mFaces[i].mIndices[0];
            pIndices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
            pIndices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
        }
    }

    return pIndices;
}

