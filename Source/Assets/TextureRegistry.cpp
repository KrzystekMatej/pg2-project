#include "Assets/TextureRegistry.h"

#include <glad/gl.h>
#include <FreeImage.h>


const Texture* TextureRegistry::CreateSolidTexture(const glm::vec3 color)
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

	return AddAsset(textureName, std::make_unique<Texture>(id, handle, 1, 1, TextureFormat::RGBA8));
}

const Texture* TextureRegistry::LoadTexture(const std::filesystem::path& filePath, const std::string& textureName)
{
	if (const Texture* cached = GetAsset(textureName)) return cached;

	std::string filePathStr = filePath.string();

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filePathStr.c_str(), 0);
	if (format == FIF_UNKNOWN) format = FreeImage_GetFIFFromFilename(filePathStr.c_str());
	if (format == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(format))
	{
		spdlog::error("LoadMipChain: Unsupported file format '{}'", filePathStr);
		return nullptr;
	}

	FIBITMAP* dib = FreeImage_Load(format, filePathStr.c_str());
	if (!dib)
	{
		spdlog::error("LoadTexture: Failed to load image '{}'", filePathStr);
		return nullptr;
	}


	FIBITMAP* converted = FreeImage_ConvertTo32Bits(dib);
	FreeImage_Unload(dib);

	if (!converted)
	{
		spdlog::error("LoadTexture: Failed to convert image to 32-bit '{}'", filePathStr);
		return nullptr;
	}

	BYTE* pixels = FreeImage_GetBits(converted);

	if (!pixels)
	{
		spdlog::error("LoadTexture: Failed to get pixel data from '{}'", filePathStr);
		FreeImage_Unload(converted);
		return nullptr;
	}

	uint32_t width = FreeImage_GetWidth(converted);
	uint32_t height = FreeImage_GetHeight(converted);

	uint32_t id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<int>(width), static_cast<int>(height), 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(converted);
	glBindTexture(GL_TEXTURE_2D, 0);

	uint64_t handle = glGetTextureHandleARB(id);
	glMakeTextureHandleResidentARB(handle);

	return AddAsset(textureName, std::make_unique<Texture>(id, handle, width, height, TextureFormat::RGBA8));
}

const Texture* TextureRegistry::LoadMipChain(const std::filesystem::path& directoryPath, const std::string& textureName, TextureFormat textureFormat)
{
	if (const Texture* cached = GetAsset(textureName)) return cached;

	if (!exists(directoryPath) || !is_directory(directoryPath))
	{
		spdlog::error("LoadMipChain: Invalid directory '{}'", directoryPath.string());
		return nullptr;
	}

	std::vector<std::filesystem::path> mipPaths;

	for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
	{
		if (entry.is_regular_file()) mipPaths.push_back(entry.path());
	}

	if (mipPaths.empty())
	{
		spdlog::warn("LoadMipChain: No mip files found in '{}'", directoryPath.string());
		return nullptr;
	}

	std::ranges::sort(mipPaths);

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(mipPaths.size() - 1));

	GLint internalFormat = GL_RGB32F;
	GLenum dataFormat = GL_RGB;
	GLenum dataType = GL_FLOAT;

	if (textureFormat == TextureFormat::RGB16F) internalFormat = GL_RGB16F;
	else if (textureFormat == TextureFormat::RGBA8)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_BGRA;
		dataType = GL_UNSIGNED_BYTE;
	}

	uint32_t width = 0;
	uint32_t height = 0;

	for (size_t level = 0; level < mipPaths.size(); ++level)
	{
		const std::string& filePathStr = mipPaths[level].string();

		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filePathStr.c_str(), 0);
		if (format == FIF_UNKNOWN) format = FreeImage_GetFIFFromFilename(filePathStr.c_str());
		if (format == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(format))
		{
			spdlog::error("LoadMipChain: Unsupported file format '{}'", filePathStr);
			return nullptr;
		}

		FIBITMAP* dib = FreeImage_Load(format, filePathStr.c_str());
		if (!dib)
		{
			spdlog::error("LoadMipChain: Failed to load mip level {} ('{}')", level, filePathStr);
			return nullptr;
		}

		FIBITMAP* converted = textureFormat == TextureFormat::RGBA8 ? FreeImage_ConvertTo32Bits(dib) : FreeImage_ConvertToRGBF(dib);
		FreeImage_Unload(dib);

		if (!converted)
		{
			spdlog::error("LoadMipChain: Failed to convert mip level {} ('{}')", level, filePathStr);
			return nullptr;
		}

		BYTE* pixels = FreeImage_GetBits(converted);

		if (!pixels)
		{
			spdlog::error("LoadMipChain: Failed to get pixel data from mip level {} ('{}')", level, filePathStr);
			FreeImage_Unload(converted);
			return nullptr;
		}

		width = FreeImage_GetWidth(converted);
		height = FreeImage_GetHeight(converted);

		glTexImage2D(GL_TEXTURE_2D, static_cast<int>(level), internalFormat, static_cast<int>(width), static_cast<int>(height),0, dataFormat, dataType, pixels);

		FreeImage_Unload(converted);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	uint64_t handle = glGetTextureHandleARB(id);
	glMakeTextureHandleResidentARB(handle);

	return AddAsset(textureName, std::make_unique<Texture>(id, handle, width, height, TextureFormat::RGBA8));
}
