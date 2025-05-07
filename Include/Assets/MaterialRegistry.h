#pragma once
#include <string>
#include "AssetRegistry.h"
#include "ShaderRegistry.h"
#include "Renderer/MaterialAsset.h"
#include "Assets/TextureRegistry.h"
#include <tiny_obj_loader.h>

#include "Renderer/ShaderStorageBuffer.h"

namespace MaterialKeys
{
	constexpr std::string_view Shader = "shader";

	constexpr std::string_view Ior = "Ni";
	constexpr std::string_view Reflectivity = "Pspec";
	constexpr std::string_view Attenuation = "Tf";

	constexpr std::string_view Diffuse = "Kd";
	constexpr std::string_view Specular = "Ks";
	constexpr std::string_view Shininess = "Ns";
	constexpr std::string_view Ambient = "Ka";
	constexpr std::string_view Roughness = "Pr";
	constexpr std::string_view Metallic = "Pm";
	constexpr std::string_view Bump = "bump";
	constexpr std::string_view Opacity = "d";
	constexpr std::string_view Emission = "Ke";

	constexpr std::string_view MapDiffuse = "map_Kd";
	constexpr std::string_view MapSpecular = "map_Ks";
	constexpr std::string_view MapGlossiness = "map_Ps";
	constexpr std::string_view MapBaseColor = "map_BaseColor";
	constexpr std::string_view MapRoughness = "map_Pr";
	constexpr std::string_view MapMetallic = "map_Pm";
	constexpr std::string_view MapRMA = "map_RMA";
	constexpr std::string_view MapBump = "map_Bump";
	constexpr std::string_view MapOpacity = "map_d";
	constexpr std::string_view MapEmission = "map_Ke";
}

struct MaterialRange
{
	uint32_t Offset;
	uint32_t Count;
};

class MaterialRegistry : public AssetRegistry<MaterialRange>
{
public:
	using BaseRegistryType = AssetRegistry;
	using AssetRegistry::AssetRegistry;

	MaterialRegistry(uint32_t initialCapacity)
		: m_Buffer(ShaderStorageBuffer::Material, initialCapacity,sizeof(GLMaterial)) {}

	const ShaderStorageBuffer* GetMaterialBuffer() const { return &m_Buffer; }

	const MaterialRange* LoadMaterialRange
	(
		const std::vector<tinyobj::material_t>& materialSources,
		const std::string& rangeName,
		const std::filesystem::path& directoryPath,
		TextureRegistry* textureRegistry
	);

	static const std::string& GetCustomParameter(const tinyobj::material_t& materialSource, const std::string& key);
	static const Texture* GetTexture(const std::filesystem::path& directoryPath, const std::string& textureFile, TextureRegistry* textureRegistry, const glm::vec3& fallback);
private:
	static MaterialAsset LoadMaterial(const tinyobj::material_t& materialSource, const std::string& materialName, const std::filesystem::path& directoryPath, TextureRegistry* textureRegistry);
	ShaderStorageBuffer m_Buffer;
	uint32_t m_Offset = 0;
};
