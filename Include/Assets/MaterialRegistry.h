#pragma once
#include <string>
#include <assimp/scene.h>
#include "AssetRegistry.h"
#include "Renderer/MaterialAsset.h"
#include "Assets/TextureRegistry.h"

class MaterialRegistry : public AssetRegistry<MaterialAsset>
{
public:
	using BaseRegistryType = AssetRegistry;

	const MaterialAsset* LoadMaterial(const aiMaterial* aiMaterial, const std::string& materialName, TextureRegistry* textureRegistry);
};
