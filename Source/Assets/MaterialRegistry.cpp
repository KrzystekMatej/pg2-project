#include "Assets/MaterialRegistry.h"
#include "Assets/TextureRegistry.h"
#include "Assets/AssetManager.h"

const MaterialAsset* MaterialRegistry::LoadMaterial(const aiMaterial* aiMaterial, const std::string& materialName, TextureRegistry* textureRegistry)
{
	/*if (const MaterialAsset* cached = GetAsset(materialName)) return cached;

	auto material = std::make_unique<MaterialAsset>();
	material->Name = materialName;

	float shininess = 0.0f;
	if (aiMaterial->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
		material->Shininess = shininess;

	float metallic = 0.0f;
	if (aiMaterial->Get("metallicFactor", 0, 0, metallic) == AI_SUCCESS)
		material->Metallic = metallic;

	float roughness = 0.0f;
	if (aiMaterial->Get("roughnessFactor", 0, 0, roughness) == AI_SUCCESS)
		material->Roughness = roughness;

	aiColor3D emission(0.f, 0.f, 0.f);
	if (aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emission) == AI_SUCCESS)
		material->Emission = glm::vec3(emission.r, emission.g, emission.b);

	aiString aiTexturePath;

	if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS && texturePath.length > 0)
	{
		std::filesystem::path aiTexturePath = 
		const Texture* texture = textureRegistry->LoadTexture(texturePath, texturePath.stem().string(), TextureType::Color);
		if (texture) material->SetTexture(Map::kDiffuse, texture);
	}

	if (aiMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath) == AI_SUCCESS && texturePath.length > 0)
	{
		const Texture* texture = textureRegistry->LoadTexture(texturePath, texturePath.stem().string(), TextureType::Generic);
		if (texture) material->SetTexture(Map::kSpecular, texture);
	}

	if (aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS && texturePath.length > 0)
	{
		const Texture* texture = LoadTextureFromPath(textureRegistry, texturePath.C_Str(), TextureType::Normal);
		if (texture) material->SetTexture(Map::kNormal, texture);
	};

	if (aiMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath) == AI_SUCCESS && texturePath.length > 0)
	{
		const Texture* texture = LoadTextureFromPath(textureRegistry, texturePath.C_Str(), TextureType::Emission);
		if (texture) material->SetTexture(Map::kEmission, texture);
	}

	return AddAsset(materialName, std::move(material));*/
	return nullptr;
}
