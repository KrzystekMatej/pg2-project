#include <glad/gl.h>
#include "Assets/TextureRegistry.h"

#include <FreeImage.h>


const Texture* TextureRegistry::CreateEmptyTexture(uint32_t width, uint32_t height)
{
	if (width <= 0 || height <= 0) return nullptr;

	std::string textureName = std::format("empty_{}x{}", width, height);
	if (const Texture* cached = GetAsset(textureName)) return cached;

	uint32_t id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);

	glBindTexture(GL_TEXTURE_2D, 0);

	uint64_t handle = glGetTextureHandleARB(id);
	glMakeTextureHandleResidentARB(handle);

	return AddAsset(textureName, std::make_unique<Texture>(id, handle, width, height));
}

const Texture* TextureRegistry::CreatePixelTexture(glm::vec3 color)
{
	uint8_t pixel[4] =
	{
		static_cast<uint8_t>(glm::clamp(color.r, 0.0f, 1.0f) * 255.0f),
		static_cast<uint8_t>(glm::clamp(color.g, 0.0f, 1.0f) * 255.0f),
		static_cast<uint8_t>(glm::clamp(color.b, 0.0f, 1.0f) * 255.0f),
		255
	};

	std::string textureName = std::format("({},{},{})", pixel[0], pixel[1], pixel[2]);

	if (const Texture* cached = GetAsset(textureName)) return cached;

	uint32_t id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	glBindTexture(GL_TEXTURE_2D, 0);

	uint64_t handle = glGetTextureHandleARB(id);
	glMakeTextureHandleResidentARB(handle);

	return AddAsset(textureName, std::make_unique<Texture>(id, handle, 1, 1));
}

const Texture* TextureRegistry::CreateTexture(const GLImage& image, const std::string& textureName)
{
	if (const Texture* cached = GetAsset(textureName)) return cached;

	BYTE* pixels = FreeImage_GetBits(image.Bitmap);
	if (!pixels)
	{
		spdlog::error("CreateTexture: Failed to get pixel data for '{}'", textureName);
		return nullptr;
	}

	uint32_t id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D
	(
		GL_TEXTURE_2D,
		0,
		image.InternalFormat,
		static_cast<int>(image.Width),
		static_cast<int>(image.Height),
		0,
		image.ExternalFormat,
		image.DataType,
		pixels
	);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	uint64_t handle = glGetTextureHandleARB(id);
	glMakeTextureHandleResidentARB(handle);

	return AddAsset(textureName, std::make_unique<Texture>(id, handle, image.Width, image.Height));
}

const Texture* TextureRegistry::CreateMipChain(const std::vector<std::unique_ptr<GLImage>>& mips, const std::string& textureName)
{
	if (const Texture* cached = GetAsset(textureName)) return cached;

	if (mips.empty())
	{
		spdlog::warn("CreateMipChain: Missing mip files for '{}'", textureName);
		return {};
	}

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(mips.size() - 1));

	uint32_t width = 0;
	uint32_t height = 0;

	for (size_t i = 0; i < mips.size(); i++)
	{
		const auto& image = mips[i];

		BYTE* pixels = FreeImage_GetBits(image->Bitmap);

		if (!pixels)
		{
			spdlog::error("CreateMipChain: Failed to get pixel data from mip level {} ('{}')", i, textureName);
			return nullptr;
		}

		width = FreeImage_GetWidth(image->Bitmap);
		height = FreeImage_GetHeight(image->Bitmap);

		glTexImage2D(GL_TEXTURE_2D, static_cast<int>(i), image->InternalFormat, static_cast<int>(image->Width), static_cast<int>(image->Height),0, image->ExternalFormat, image->DataType, pixels);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	uint64_t handle = glGetTextureHandleARB(id);
	glMakeTextureHandleResidentARB(handle);

	return AddAsset(textureName, std::make_unique<Texture>(id, handle, width, height));
}
