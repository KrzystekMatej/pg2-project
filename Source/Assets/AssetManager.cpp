#include "Assets/AssetManager.h"
#include <spdlog/spdlog.h>
#include "Assets/MeshRegistry.h"
#include "Assets/ShaderRegistry.h"
#include "Assets/MaterialRegistry.h"
#include <tiny_obj_loader.h>

void AssetManager::Initialize()
{
	AddRegistry<ShaderRegistry>();
	AddRegistry<MeshRegistry>();
	AddRegistry<MaterialRegistry>(256);
	AddRegistry<TextureRegistry>();
}

const MeshHandle* AssetManager::LoadObjFile(const std::filesystem::path& filePath) const
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materialSources;
	std::string warn, err;

	const std::filesystem::path baseDirectory = filePath.parent_path();

	bool ok = tinyobj::LoadObj(
		&attrib,
		&shapes,
		&materialSources,
		&warn,
		&err,
		filePath.string().c_str(),
		baseDirectory.string().c_str(),
		true
	);

	if (!ok)
	{
		spdlog::error("TinyOBJ failed to load obj '{}': {}", filePath.string(), err);
		return {};
	}

	std::string directoryName = baseDirectory.filename().string();

	const MaterialRange* range = GetRegistry<MaterialRegistry>()->LoadMaterialRange(materialSources, directoryName, baseDirectory, GetRegistry<TextureRegistry>());
	return GetRegistry<MeshRegistry>()->LoadMesh(shapes, attrib, directoryName, range);
}
