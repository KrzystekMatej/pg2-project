#include "Renderer/Texture.h"
#include <FreeImage.h>

Texture::~Texture()
{
	if (m_Id)
	{
		glDeleteTextures(1, &m_Id);
		m_Id = 0;
	}
}

void Texture::Bind(uint32_t unit) const
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, m_Id);
}

std::shared_ptr<Texture> Texture::CreateFromFile(const std::string& path, TextureType type)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
	if (format == FIF_UNKNOWN) format = FreeImage_GetFIFFromFilename(path.c_str());

	if (format == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(format)) return nullptr;

	FIBITMAP* dib = FreeImage_Load(format, path.c_str());
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

	return std::make_shared<Texture>(id, width, height, TextureFormat::RGBA8, type);
}

std::shared_ptr<Texture> Texture::CreateFromMipChain(const std::vector<std::string>& paths, TextureFormat format, TextureType type)
{
	if (paths.empty())
		return nullptr;

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(paths.size()) - 1);

	GLint internalFormat = GL_RGB32F;
	GLenum dataFormat = GL_RGB;
	GLenum dataType = GL_FLOAT;

	if (format == TextureFormat::RGB16F)
	{
		internalFormat = GL_RGB16F;
	}
	else if (format == TextureFormat::RGBA8)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_BGRA;
		dataType = GL_UNSIGNED_BYTE;
	}

	uint32_t width = 0;
	uint32_t height = 0;

	for (size_t level = 0; level < paths.size(); ++level)
	{
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(paths[level].c_str(), 0);
		if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(paths[level].c_str());
		if (fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fif)) return nullptr;

		FIBITMAP* dib = FreeImage_Load(fif, paths[level].c_str());
		if (!dib) return nullptr;

		FIBITMAP* converted = (format == TextureFormat::RGBA8) ? FreeImage_ConvertTo32Bits(dib) : FreeImage_ConvertToRGBF(dib);
		FreeImage_Unload(dib);

		BYTE* pixels = FreeImage_GetBits(converted);
		width = FreeImage_GetWidth(converted);
		height = FreeImage_GetHeight(converted);

		glTexImage2D(GL_TEXTURE_2D, static_cast<int>(level), internalFormat, static_cast<int>(width), static_cast<int>(height), 0, dataFormat, dataType, pixels);

		FreeImage_Unload(converted);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	return std::make_shared<Texture>(id, width, height, format, type);
}

