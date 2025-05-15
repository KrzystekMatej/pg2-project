#pragma once
#include <string>
#include <filesystem>
#include "AssetRegistry.h"
#include "Renderer/ShaderProgram.h"

class ShaderRegistry : public AssetRegistry<ShaderProgram>
{
public:
	using BaseRegistryType = AssetRegistry;
	using AssetRegistry::AssetRegistry;

	const ShaderProgram* LoadShaderProgram
	(
		const std::filesystem::path& directoryPath,
		const std::string& shaderName,
		ShaderType shaderType
	);
};