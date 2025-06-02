#include <glad/glad.h>
#include "Assets/GLImage.h"
#include <string>
#include <spdlog/spdlog.h>

std::unique_ptr<GLImage> GLImage::LoadImage(const std::filesystem::path& filePath, ColorSpace colorSpace, bool verticalFlip, bool logError)
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

	if (verticalFlip) FreeImage_FlipVertical(bitmap);

	const FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bitmap);
	uint32_t width = FreeImage_GetWidth(bitmap);
	uint32_t height = FreeImage_GetHeight(bitmap);

	int32_t  internalFormat = 0;
	int32_t  externalFormat = 0;
	uint32_t dataType = 0;
	ImageFormat myFormat = ImageFormat::Unknown;

	switch (imageType)
	{
		case FIT_FLOAT:
			internalFormat = GL_R32F;
			externalFormat = GL_RED;
			dataType = GL_FLOAT;

			myFormat = ImageFormat::F32_R;
			break;

		case FIT_RGBF:
			internalFormat = GL_RGB32F;
			externalFormat = GL_RGB;
			dataType = GL_FLOAT;

			myFormat = ImageFormat::F32_RGB;
			break;

		case FIT_RGBAF:
			internalFormat = GL_RGBA32F;
			externalFormat = GL_RGBA;
			dataType = GL_FLOAT;

			myFormat = ImageFormat::F32_RGBA;
			break;

		case FIT_UINT16:
		case FIT_INT16:
			internalFormat = GL_R16;
			externalFormat = GL_RED;
			dataType = GL_UNSIGNED_SHORT;

			myFormat = ImageFormat::U16_R;
			break;
		case FIT_UINT32:
		case FIT_INT32:
			internalFormat = GL_R32UI;
			externalFormat = GL_RED_INTEGER;
			dataType = GL_UNSIGNED_INT;

			myFormat = ImageFormat::U32_R;
			break;

		case FIT_BITMAP:
		default:
		{
			const unsigned bpp = FreeImage_GetBPP(bitmap);
			switch (bpp)
			{
				case 8:
					myFormat = ImageFormat::U8_R;
					internalFormat = GL_R8;
					externalFormat = GL_RED;
					dataType = GL_UNSIGNED_BYTE;
					break;
				case 24:
				case 32:
				{
					FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
					if (!converted)
					{
						FreeImage_Unload(bitmap);
						spdlog::error("LoadImage: Failed to convert image to 32-bit '{}'",
							filePathStr);
						return nullptr;
					}
					FreeImage_Unload(bitmap);
					bitmap = converted;

					myFormat = ImageFormat::U8_RGBA;
					externalFormat = GL_BGRA;
					dataType = GL_UNSIGNED_BYTE;
					internalFormat = colorSpace == ColorSpace::SRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
					break;
				}
				default:
					FreeImage_Unload(bitmap);
					spdlog::error("LoadImage: Unsupported BPP {} in '{}'", bpp, filePathStr);
					return nullptr;
			}

			break;
		}
	}

	return std::make_unique<GLImage>(bitmap, width, height, dataType, internalFormat, externalFormat, myFormat);
}

std::vector<std::unique_ptr<GLImage>> GLImage::LoadMipChain(const std::filesystem::path& directoryPath, ColorSpace colorSpace)
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
		if (std::unique_ptr<GLImage> image = LoadImage(path, colorSpace)) mips.push_back(std::move(image));
	}

	if (mips.empty())
	{
		spdlog::warn("LoadMipChain: No mip files found in '{}'", directoryPath.string());
		return {};
	}

	return mips;
}