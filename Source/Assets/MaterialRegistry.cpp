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
	material.SetFallback(Map::RMA, glm::vec3(materialSource.roughness, materialSource.metallic, AO_DEFAULT));
	material.SetFallback(Map::Opacity, glm::vec3(materialSource.dissolve));
	material.SetFallback(Map::Emission, glm::vec3(materialSource.emission[0], materialSource.emission[1], materialSource.emission[2]));


	material.SetTexture(Map::Diffuse, GetTextureOrPixel(directoryPath, materialSource.diffuse_texname, textureRegistry, material.GetFallback(Map::Diffuse)));
	material.SetTexture(Map::Specular, GetTextureOrPixel(directoryPath, materialSource.specular_texname, textureRegistry, material.GetFallback(Map::Specular)));
	material.SetTexture(Map::Glossiness, GetTextureOrPixel(directoryPath, materialSource.sheen_texname, textureRegistry, material.GetFallback(Map::Glossiness)));

	material.SetTexture(Map::RMA, GetRMATexture(materialSource, material, directoryPath, textureRegistry));

	material.SetTexture(Map::Normal, GetTextureOrPixel(directoryPath, materialSource.bump_texname, textureRegistry, material.GetFallback(Map::Normal)));
	material.SetTexture(Map::Opacity, GetTextureOrPixel(directoryPath, materialSource.alpha_texname, textureRegistry, material.GetFallback(Map::Opacity)));
	material.SetTexture(Map::Emission, GetTextureOrPixel(directoryPath, materialSource.emissive_texname, textureRegistry, material.GetFallback(Map::Emission)));
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

const Texture* MaterialRegistry::GetTextureOrPixel(const std::filesystem::path& directoryPath, const std::string& textureFile, TextureRegistry* textureRegistry, const glm::vec3& fallback)
{
	const Texture* texture = GetTexture(directoryPath, textureFile, textureRegistry);
	if (!texture) return textureRegistry->CreatePixelTexture(fallback);
	return  texture;
}


const Texture* MaterialRegistry::GetTexture(const std::filesystem::path& directoryPath, const std::string& textureFile, TextureRegistry* textureRegistry)
{
	if (textureFile.empty()) return nullptr;
	std::filesystem::path texturePath = directoryPath / textureFile;
	if (exists(texturePath))
	{
		const Texture* texture = nullptr;
		if (is_regular_file(texturePath))
		{
			std::unique_ptr<GLImage> image = GLImage::LoadImage(texturePath);
			if (!image) return nullptr;
			texture = textureRegistry->CreateTexture(*image, texturePath.stem().string());
		}
		else if (is_directory(texturePath))
		{
			std::vector<std::unique_ptr<GLImage>> mips = GLImage::LoadMipChain(texturePath);
			if (mips.empty()) return nullptr;
			texture = textureRegistry->CreateMipChain(mips, texturePath.filename().string());
		}
		if (texture) return texture;
	}
	return nullptr;
}

const Texture* MaterialRegistry::GetRMATexture
(
	const tinyobj::material_t& materialSource,
	const MaterialAsset& material,
	const std::filesystem::path& directoryPath,
	TextureRegistry* textureRegistry
)
{
	const std::string& rmaTextureFile = GetCustomParameter(materialSource, std::string(MaterialKeys::MapRMA));
	const Texture* texture = GetTexture(directoryPath, rmaTextureFile, textureRegistry);
	if (texture) return texture;

	std::filesystem::path roughPath = directoryPath / materialSource.roughness_texname;
	std::filesystem::path metalPath = directoryPath / materialSource.metallic_texname;
	std::filesystem::path aoPath = directoryPath / materialSource.ambient_texname;

	std::unique_ptr<GLImage> roughImage = GLImage::LoadImage(roughPath, false);
	std::unique_ptr<GLImage> metalImage = GLImage::LoadImage(metalPath, false);
	std::unique_ptr<GLImage> aoImage = GLImage::LoadImage(aoPath, false);

	if (!roughImage) { spdlog::warn("Roughness texture missing: replaced with fallback."); }
	if (!metalImage) { spdlog::warn("Metallic texture missing: replaced with fallback."); }
	if (!aoImage) { spdlog::warn("Ambient occlusion texture missing: replaced with fallback."); }

	uint32_t width = 0;
	uint32_t height = 0;

	if (roughImage) { width = roughImage->Width; height = roughImage->Height; }
	else if (metalImage) { width = metalImage->Width; height = metalImage->Height; }
	else if (aoImage) { width = aoImage->Width; height = aoImage->Height; }

	glm::vec3 rmaFallback = material.GetFallback(Map::RMA);

	if (width == 0 || height == 0)
	{
		spdlog::warn("RMA generation failed: creating pixel fallback.");
		return textureRegistry->CreatePixelTexture(rmaFallback);
	}

	bool isValid = ValidateRMAMerge(roughImage, width, height, roughPath) && ValidateRMAMerge(metalImage, width, height, metalPath) && ValidateRMAMerge(aoImage, width, height, aoPath);

	if (!isValid) return textureRegistry->CreatePixelTexture(rmaFallback);

	std::unique_ptr<GLImage> rmaImage = GenerateRMAImage(width, height, roughImage.get(), metalImage.get(), aoImage.get(), rmaFallback);
	if (!rmaImage) return textureRegistry->CreatePixelTexture(rmaFallback);

	std::string textureName = directoryPath.filename().string() + "_RMA";
	return textureRegistry->CreateTexture(*rmaImage, textureName);
}

bool MaterialRegistry::ValidateRMAMerge(const std::unique_ptr<GLImage>& image, uint32_t expectedWidth, uint32_t expectedHeight, const std::filesystem::path& filePath)
{
	if (!image) return true;

	if (image->InternalFormat != GL_R32F || image->ExternalFormat != GL_RED || image->DataType != GL_FLOAT)
	{
		spdlog::error("RMA generation failed: '{}' is not single-channel float (GL_R32F).", filePath.string());
		return false;
	}

	if (image->Width != expectedWidth || image->Height != expectedHeight)
	{
		spdlog::error("RMA generation failed: '{}' resolution mismatch.", filePath.string());
		return false;
	}

	return true;
}

std::unique_ptr<GLImage> MaterialRegistry::GenerateRMAImage
(
	uint32_t width,
	uint32_t height,
	const GLImage* roughImage,
	const GLImage* metalImage,
	const GLImage* aoImage,
	glm::vec3 rmaFallback
)
{
	FIBITMAP* bitmap = FreeImage_AllocateT(FIT_RGBF, width, height);
	if (!bitmap)
	{
		spdlog::error("RMA generation failed: failed to allocate target bitmap.");
		return nullptr;
	}

	float* rmaPixels = reinterpret_cast<float*>(FreeImage_GetBits(bitmap));

	const float* roughPixels = roughImage ? reinterpret_cast<const float*>(FreeImage_GetBits(roughImage->Bitmap)) : nullptr;
	const float* metalPixels = metalImage ? reinterpret_cast<const float*>(FreeImage_GetBits(metalImage->Bitmap)) : nullptr;
	const float* aoPixels = aoImage ? reinterpret_cast<const float*>(FreeImage_GetBits(aoImage->Bitmap)) : nullptr;

	const size_t pixelCount = static_cast<size_t>(width) * height;
	for (size_t i = 0; i < pixelCount; ++i)
	{
		float rough = roughPixels ? roughPixels[i] : rmaFallback.r;
		float metal = metalPixels ? metalPixels[i] : rmaFallback.g;
		float ao = aoPixels ? aoPixels[i] : rmaFallback.b;

		rmaPixels[i * 3 + 0] = rough;
		rmaPixels[i * 3 + 1] = metal;
		rmaPixels[i * 3 + 2] = ao;
	}

	return std::make_unique<GLImage>(
		bitmap,
		width,
		height,
		GL_FLOAT,
		GL_RGB32F,
		GL_RGB
	);
}


