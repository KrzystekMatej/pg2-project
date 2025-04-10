#include "Assets/AssetManager.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <spdlog/spdlog.h>
#include "Assets/MeshRegistry.h"
#include "Assets/ShaderRegistry.h"

std::vector<const MeshHandle*> AssetManager::LoadObjFile(const std::filesystem::path& directoryPath, const std::string& fileName)
{
    std::filesystem::path fullPath = directoryPath / fileName;

    uint32_t importOptions = aiProcess_OptimizeMeshes
        | aiProcess_JoinIdenticalVertices
        | aiProcess_Triangulate
        | aiProcess_CalcTangentSpace;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fullPath.string(), importOptions);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        spdlog::error("Assimp failed to load mesh '{}': {}", fileName, importer.GetErrorString());
        return {};
    }

    std::vector<const MeshHandle*> result;
    MeshRegistry* meshRegistry = GetRegistry<MeshRegistry>();
    if (!meshRegistry) return {};

    uint32_t meshIndex = 0;
    for (uint32_t i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[i];

        std::string meshName = fileName + "_mesh" + std::to_string(meshIndex++);
        const MeshHandle* handle = meshRegistry->LoadMesh(mesh, meshName);

        if (handle) result.emplace_back(handle);
    }

    return result;
}