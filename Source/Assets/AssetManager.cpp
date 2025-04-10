#include "Assets/AssetManager.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <spdlog/spdlog.h>
#include "Assets/MeshRegistry.h"
#include "Assets/ShaderRegistry.h"
#include "Utils/Text.h"
#include "Assets/MaterialRegistry.h"

std::vector<const MeshHandle*> AssetManager::LoadObjFile(const std::filesystem::path& filePath)
{
    uint32_t importOptions = aiProcess_OptimizeMeshes
        | aiProcess_JoinIdenticalVertices
        | aiProcess_Triangulate
        | aiProcess_CalcTangentSpace;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath.string(), importOptions);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        spdlog::error("Assimp failed to load obj file '{}': {}", filePath.string(), importer.GetErrorString());
        return {};
    }

    std::vector<const MeshHandle*> result;
    MeshRegistry* meshRegistry = GetRegistry<MeshRegistry>();
    MaterialRegistry* materialRegistry = GetRegistry<MaterialRegistry>();

    for (uint32_t i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[i];

        std::string meshName;
        if (mesh->mName.length > 0) meshName = mesh->mName.C_Str();
        else meshName = GetDefaultAssetName<MeshHandle>(filePath, std::to_string(i));

        const MeshHandle* handle = meshRegistry->LoadMesh(mesh, meshName);
        if (handle) result.emplace_back(handle);

        std::string materialName;
        aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];

        aiString aiMaterialName;
        if (aiMaterial->Get(AI_MATKEY_NAME, aiMaterialName) == AI_SUCCESS && aiMaterialName.length > 0)
        {
            materialName = aiMaterialName.C_Str();
        }
        else materialName = GetDefaultAssetName<MaterialAsset>(filePath, std::to_string(mesh->mMaterialIndex));

        const MaterialAsset* material = materialRegistry->LoadMaterial(aiMaterial, materialName, GetRegistry<TextureRegistry>());
    }

    return result;
}