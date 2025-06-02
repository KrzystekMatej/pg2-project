#include <glad/glad.h>
#include "Assets/TextureRegistry.h"
#include "Assets/TextureFactory.h"
#include <FreeImage.h>

const Texture* TextureRegistry::RegisterEmptyTexture(uint32_t width, uint32_t height)
{
	std::string textureName = std::format("empty_{}x{}", width, height);
	if (const Texture* cached = GetAsset(textureName)) return cached;
	auto texture = TextureFactory::CreateEmpty(width, height);
	return AddAsset(textureName, std::move(texture));
}

const Texture* TextureRegistry::RegisterPixelTexture(glm::vec3 color)
{
	uint8_t pixel[3] =
	{
		static_cast<uint8_t>(glm::clamp(color.r, 0.0f, 1.0f) * 255.0f),
		static_cast<uint8_t>(glm::clamp(color.g, 0.0f, 1.0f) * 255.0f),
		static_cast<uint8_t>(glm::clamp(color.b, 0.0f, 1.0f) * 255.0f),
	};

	std::string textureName = std::format("({},{},{})", pixel[0], pixel[1], pixel[2]);
	if (const Texture* cached = GetAsset(textureName)) return cached;
	auto texture = TextureFactory::CreatePixel(color);
	return AddAsset(textureName, std::move(texture));
}

const Texture* TextureRegistry::RegisterImageTexture(const GLImage& image, const std::string& textureName)
{
	if (const Texture* cached = GetAsset(textureName)) return cached;
	auto texture = TextureFactory::CreateFromImage(image);
	return AddAsset(textureName, std::move(texture));
}

const Texture* TextureRegistry::RegisterMipChain(const std::vector<std::unique_ptr<GLImage>>& mips, const std::string& textureName)
{
	if (const Texture* cached = GetAsset(textureName)) return cached;
	auto texture = TextureFactory::CreateFromMipChain(mips);
	return AddAsset(textureName, std::move(texture));
}
