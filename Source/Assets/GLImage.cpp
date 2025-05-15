#include <glad/gl.h>
#include "Assets/GLImage.h"
#include <string>
#include <spdlog/spdlog.h>

std::unique_ptr<GLImage> GLImage::LoadImage(const std::filesystem::path& filePath, bool logError)
{
	std::string filePathStr = filePath.string();

	if (!is_regular_file(filePath))
	{
		if (logError) spdlog::error("LoadImage: File '{}' does not exist", filePathStr);
		return nullptr;
	}

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filePathStr.c_str(), 0);
	if (format == FIF_UNKNOWN) format = FreeImage_GetFIFFromFilename(filePathStr.c_str());
	if (format == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(format))
	{
		if (logError) spdlog::error("LoadImage: Unsupported file format '{}'", filePathStr);
		return nullptr;
	}

	FIBITMAP* bitmap = FreeImage_Load(format, filePathStr.c_str());
	if (!bitmap)
	{
		if (logError) spdlog::error("LoadImage: Failed to load image '{}'", filePathStr);
		return nullptr;
	}

	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bitmap);
	int32_t internalFormat;
	int32_t externalFormat;
	uint32_t dataType;

	uint32_t width = FreeImage_GetWidth(bitmap);
	uint32_t height = FreeImage_GetHeight(bitmap);

	switch (imageType)
	{
		case FIT_FLOAT:
			internalFormat = GL_R32F;
			externalFormat = GL_RED;
			dataType = GL_FLOAT;
			break;

		case FIT_RGBF:
			internalFormat = GL_RGB32F;
			externalFormat = GL_RGB;
			dataType = GL_FLOAT;
			break;

		case FIT_RGBAF:
			internalFormat = GL_RGBA32F;
			externalFormat = GL_RGBA;
			dataType = GL_FLOAT;
			break;

		case FIT_UINT16:
		case FIT_INT16:
			internalFormat = GL_R16;
			externalFormat = GL_RED;
			dataType = GL_UNSIGNED_SHORT;
			break;

		case FIT_UINT32:
		case FIT_INT32:
			internalFormat = GL_R32UI;
			externalFormat = GL_RED_INTEGER;
			dataType = GL_UNSIGNED_INT;
			break;

		case FIT_BITMAP:
		default:
		{
			FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
			FreeImage_Unload(bitmap);

			if (!converted)
			{
				spdlog::error("LoadImage: Failed to convert image to 32-bit '{}'", filePathStr);
				return nullptr;
			}

			bitmap = converted;
			internalFormat = GL_RGBA8;
			externalFormat = GL_BGRA;
			dataType = GL_UNSIGNED_BYTE;
			break;
		}
	}

	return std::make_unique<GLImage>(bitmap, width, height, dataType, internalFormat, externalFormat);
}

std::vector<std::unique_ptr<GLImage>> GLImage::LoadMipChain(const std::filesystem::path& directoryPath)
{
	if (!is_directory(directoryPath))
	{
		spdlog::error("LoadImage: Directory '{}' does not exist", directoryPath.string());
		return {};
	}

	std::vector<std::filesystem::path> mipPaths;

	for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
	{
		mipPaths.push_back(entry.path());
	}

	std::ranges::sort(mipPaths);
	std::vector<std::unique_ptr<GLImage>> mips;

	for (const std::filesystem::path& path : mipPaths)
	{
		if (std::unique_ptr<GLImage> image = LoadImage(path)) mips.push_back(std::move(image));
	}

	if (mips.empty())
	{
		spdlog::warn("LoadMipChain: No mip files found in '{}'", directoryPath.string());
		return {};
	}

	return mips;
}