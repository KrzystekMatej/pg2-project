#include "Assets/TextureRegistry.h"

#include <glad/glad.h>
#include <FreeImage.h>


const Texture* TextureRegistry::LoadTexture(const std::filesystem::path& filePath, const std::string& textureName, TextureType type)
{
	if (const Texture* cached = GetAsset(textureName)) return cached;

	const char* fullPath = filePath.string().c_str();

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fullPath, 0);
	if (format == FIF_UNKNOWN) format = FreeImage_GetFIFFromFilename(fullPath);

	if (format == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(format)) return nullptr;

	FIBITMAP* dib = FreeImage_Load(format, fullPath);
	if (!dib) return nullptr;

	FIBITMAP* converted = FreeImage_ConvertTo32Bits(dib);
	FreeImage_Unload(dib);

	uint32_t width = FreeImage_GetWidth(converted);
	uint32_t height = FreeImage_GetHeight(converted);
	BYTE* pixels = FreeImage_GetBits(converted);

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

	return AddAsset(textureName, std::make_unique<Texture>(id, width, height, TextureFormat::RGBA8, type));
}

const Texture* TextureRegistry::LoadMipChain(const std::filesystem::path& directoryPath, const std::string& textureName, TextureFormat format, TextureType type)
{
	if (const Texture* cached = GetAsset(textureName)) return cached;

	if (!exists(directoryPath) || !is_directory(directoryPath)) return nullptr;

	std::vector<std::filesystem::path> mipPaths;

	for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
	{
		if (entry.is_regular_file()) mipPaths.push_back(entry.path());
	}

	if (mipPaths.empty()) return nullptr;

	std::sort(mipPaths.begin(), mipPaths.end());

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

	if (format == TextureFormat::RGB16F) internalFormat = GL_RGB16F;
	else if (format == TextureFormat::RGBA8)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_BGRA;
		dataType = GL_UNSIGNED_BYTE;
	}

	uint32_t width = 0;
	uint32_t height = 0;

	for (size_t level = 0; level < mipPaths.size(); ++level)
	{
		const std::string& filePath = mipPaths[level].string();

		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filePath.c_str(), 0);
		if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(filePath.c_str());
		if (fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fif)) return nullptr;

		FIBITMAP* dib = FreeImage_Load(fif, filePath.c_str());
		if (!dib) return nullptr;

		FIBITMAP* converted = format == TextureFormat::RGBA8 ? FreeImage_ConvertTo32Bits(dib) : FreeImage_ConvertToRGBF(dib);

		FreeImage_Unload(dib);

		BYTE* pixels = FreeImage_GetBits(converted);
		width = FreeImage_GetWidth(converted);
		height = FreeImage_GetHeight(converted);

		glTexImage2D(GL_TEXTURE_2D, static_cast<int>(level), internalFormat, static_cast<int>(width), static_cast<int>(height),0, dataFormat, dataType, pixels);

		FreeImage_Unload(converted);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	return AddAsset(textureName, std::make_unique<Texture>(id, width, height, TextureFormat::RGBA8, type));
}
