#pragma once
#include <filesystem>
#include <memory>
#include <FreeImage.h>

enum class ImageFormat
{
	U8_R,
	U8_RGB,
	U8_RGBA,
	U16_R,
	U32_R,
	F32_R,
	F32_RGB,
	F32_RGBA,
	Unknown
};

enum class ColorSpace
{
	Linear,
	SRGB
};


struct GLImage
{
	GLImage(FIBITMAP* bitmap, uint32_t width, uint32_t height, uint32_t dataType, int32_t internalFormat, int32_t externalFormat, ImageFormat format)
		: Bitmap(bitmap), Width(width), Height(height), DataType(dataType), InternalFormat(internalFormat), ExternalFormat(externalFormat), Format(format) {}

	GLImage(GLImage&& other) noexcept
	:	Bitmap(other.Bitmap),
		Width(other.Width),
		Height(other.Height),
		DataType(other.DataType),
		InternalFormat(other.InternalFormat),
		ExternalFormat(other.ExternalFormat),
		Format(other.Format)
	{
		other.Bitmap = nullptr;
	}

	GLImage(const GLImage&) = delete;
	GLImage& operator=(const GLImage&) = delete;

	GLImage& operator=(GLImage&& other) noexcept
	{
		if (this != &other)
		{
			if (Bitmap) FreeImage_Unload(Bitmap);

			Bitmap = other.Bitmap;
			Width = other.Width;
			Height = other.Height;
			DataType = other.DataType;
			InternalFormat = other.InternalFormat;
			ExternalFormat = other.ExternalFormat;
			Format = other.Format;
			other.Bitmap = nullptr;
		}
		return *this;
	}

	~GLImage()
	{
		if (Bitmap) FreeImage_Unload(Bitmap);
	}

	static std::unique_ptr<GLImage> LoadImage(const std::filesystem::path& filePath, ColorSpace colorSpace = ColorSpace::Linear, bool logError = true);
	static std::vector<std::unique_ptr<GLImage>> LoadMipChain(const std::filesystem::path& directoryPath, ColorSpace colorSpace);

	FIBITMAP* Bitmap;
	uint32_t Width;
	uint32_t Height;
	uint32_t DataType;

	int32_t InternalFormat;
	int32_t ExternalFormat;
	ImageFormat Format = ImageFormat::Unknown;
};