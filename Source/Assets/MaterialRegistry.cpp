#include "Assets/MaterialRegistry.h"
#include "Assets/AssetManager.h"

const MaterialRange* MaterialRegistry::LoadMaterialRange
(
	const std::vector<tinyobj::material_t>& materialSources,
	const std::string& rangeName,
	const std::filesystem::path& directoryPath,
	TextureRegistry* textureRegistry
)
{
	if (const MaterialRange* cached = GetAsset(rangeName)) return cached;

	std::vector<MaterialAsset> materials;
	materials.reserve(materialSources.size());

	for (size_t materialIndex = 0; materialIndex < materialSources.size(); materialIndex++)
	{
		const tinyobj::material_t& materialSource = materialSources[materialIndex];
		std::string materialName = materialSource.name.empty() ? AssetManager::GetDefaultAssetName<MaterialAsset>(rangeName, std::to_string(materialIndex)) : materialSource.name;
		MaterialAsset material = LoadMaterial(materialSource, materialName, directoryPath, textureRegistry);
		materials.emplace_back(material);
	}

	std::vector<GLMaterial> glMaterials;
	glMaterials.reserve(materials.size());

	for (const auto& material : materials)
	{
		glMaterials.push_back(material.ToGLMaterial());
	}


	m_Buffer.SetData(glMaterials.data(), glMaterials.size() * sizeof(GLMaterial), m_Offset * sizeof(GLMaterial));
	std::unique_ptr<MaterialRange> materialRange = std::make_unique<MaterialRange>(m_Offset, glMaterials.size());

	m_Offset += glMaterials.size();

	return AddAsset(rangeName, std::move(materialRange));
}

MaterialAsset MaterialRegistry::LoadMaterial
(
	const tinyobj::material_t& materialSource,
	const std::string& name,
	const std::filesystem::path& directoryPath,
	TextureRegistry* textureRegistry
)
{
	MaterialAsset material;
	material.Name = name;

	material.Shininess = materialSource.shininess;
	material.Roughness = materialSource.roughness;
	material.Metallic = materialSource.metallic;
	material.Ior = materialSource.ior;

	const std::string& reflectivityStr = GetCustomParameter(materialSource, std::string(MaterialKeys::Reflectivity));
	if (!reflectivityStr.empty()) material.Reflectivity = std::stof(reflectivityStr);

	material.SetFallback(Map::Diffuse, glm::vec3(materialSource.diffuse[0], materialSource.diffuse[1], materialSource.diffuse[2]));
	material.SetFallback(Map::Specular, glm::vec3(materialSource.specular[0], materialSource.specular[1], materialSource.specular[2]));
	material.SetFallback(Map::Glossiness, glm::vec3(materialSource.sheen));
	material.SetFallback(Map::Roughness, glm::vec3(materialSource.roughness));
	material.SetFallback(Map::Metallic, glm::vec3(materialSource.metallic));
	material.SetFallback(Map::Opacity, glm::vec3(materialSource.dissolve));
	material.SetFallback(Map::Emission, glm::vec3(materialSource.emission[0], materialSource.emission[1], materialSource.emission[2]));


	material.SetTexture(Map::Diffuse, GetTexture(directoryPath, materialSource.diffuse_texname, textureRegistry, material.GetFallback(Map::Diffuse)));
	material.SetTexture(Map::Specular, GetTexture(directoryPath, materialSource.specular_texname, textureRegistry, material.GetFallback(Map::Specular)));
	material.SetTexture(Map::Glossiness, GetTexture(directoryPath, materialSource.sheen_texname, textureRegistry, material.GetFallback(Map::Glossiness)));

	const std::string& baseTextureFile = GetCustomParameter(materialSource, std::string(MaterialKeys::MapBaseColor));
	material.SetTexture(Map::Base, GetTexture(directoryPath, baseTextureFile, textureRegistry, material.GetFallback(Map::Base)));

	material.SetTexture(Map::Roughness, GetTexture(directoryPath, materialSource.roughness_texname, textureRegistry, material.GetFallback(Map::Roughness)));
	material.SetTexture(Map::Metallic, GetTexture(directoryPath, materialSource.metallic_texname, textureRegistry, material.GetFallback(Map::Metallic)));

	const std::string& rmaTextureFile = GetCustomParameter(materialSource, std::string(MaterialKeys::MapRMA));
	material.SetTexture(Map::RMA, GetTexture(directoryPath, rmaTextureFile, textureRegistry, material.GetFallback(Map::RMA)));

	material.SetTexture(Map::Bump, GetTexture(directoryPath, materialSource.bump_texname, textureRegistry, material.GetFallback(Map::Bump)));
	material.SetTexture(Map::Opacity, GetTexture(directoryPath, materialSource.alpha_texname, textureRegistry, material.GetFallback(Map::Opacity)));
	material.SetTexture(Map::Emission, GetTexture(directoryPath, materialSource.emissive_texname, textureRegistry, material.GetFallback(Map::Emission)));
	return material;
}

const std::string& MaterialRegistry::GetCustomParameter(const tinyobj::material_t& materialSource, const std::string& key)
{
	std::map<std::string, std::string>::const_iterator it = materialSource.unknown_parameter.find(key);
	if (it != materialSource.unknown_parameter.end())
	{
		return it->second;
	}

	return Utils::EmptyString();
}

const Texture* MaterialRegistry::GetTexture(const std::filesystem::path& directoryPath, const std::string& textureFile, TextureRegistry* textureRegistry, const glm::vec3& fallback)
{
	if (textureFile.empty()) return textureRegistry->CreateSolidTexture(fallback);

	std::filesystem::path texturePath = directoryPath / textureFile;
	if (exists(texturePath))
	{
		const Texture* texture = nullptr;
		if (is_regular_file(texturePath))
		{
			texture = textureRegistry->LoadTexture(texturePath, texturePath.stem().string());
		}
		else if (is_directory(texturePath))
		{
			texture = textureRegistry->LoadMipChain(texturePath, texturePath.filename().string(), TextureFormat::RGB8);
		}
		if (texture) return texture;
	}

	return textureRegistry->CreateSolidTexture(fallback);
}
