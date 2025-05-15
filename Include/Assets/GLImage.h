#pragma once
#include <filesystem>
#include <memory>
#include <FreeImage.h>

struct GLImage
{
	GLImage(FIBITMAP* bitmap, uint32_t width, uint32_t height, uint32_t dataType, int32_t internalFormat, int32_t externalFormat)
		: Bitmap(bitmap), Width(width), Height(height), DataType(dataType), InternalFormat(internalFormat), ExternalFormat(externalFormat) {}

	GLImage(GLImage&& other) noexcept
	:	Bitmap(other.Bitmap),
		Width(other.Width),
		Height(other.Height),
		DataType(other.DataType),
		InternalFormat(other.InternalFormat),
		ExternalFormat(other.ExternalFormat)
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

			other.Bitmap = nullptr;
		}
		return *this;
	}

	~GLImage()
	{
		if (Bitmap) FreeImage_Unload(Bitmap);
	}

	static std::unique_ptr<GLImage> LoadImage(const std::filesystem::path& filePath, bool logError = true);
	static std::vector<std::unique_ptr<GLImage>> LoadMipChain(const std::filesystem::path& directoryPath);

	FIBITMAP* Bitmap;
	uint32_t Width;
	uint32_t Height;
	uint32_t DataType;

	int32_t InternalFormat;
	int32_t ExternalFormat;
};