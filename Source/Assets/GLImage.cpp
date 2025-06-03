#include <glad/glad.h>
#include "Assets/GLImage.h"
#include <string>
#include <spdlog/spdlog.h>
#include <OpenEXR/ImfInputFile.h>
#include <OpenEXR/ImfHeader.h>
#include <OpenEXR/ImfStringAttribute.h>
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfRgbaFile.h>

std::unique_ptr<GLImage> GLImage::LoadImage
(
	const std::filesystem::path& filePath,
	ColorSpace colorSpace,
	bool logError
)
{
	const std::string pathStr = filePath.string();
	if (!is_regular_file(filePath))
	{
		if (logError) spdlog::error("LoadImage: File '{}' does not exist", pathStr);
		return nullptr;
	}

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(pathStr.c_str(), 0);
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(pathStr.c_str());
	if (fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fif))
	{
		if (logError) spdlog::error("LoadImage: Unsupported format '{}'", pathStr);
		return nullptr;
	}

	FIBITMAP* bmp = FreeImage_Load(fif, pathStr.c_str());
	if (!bmp)
	{
		if (logError) spdlog::error("LoadImage: Can't load '{}'", pathStr);
		return nullptr;
	}


	auto exif_orientation = [&]()
		{
			FITAG* tag = nullptr;
			if (FreeImage_GetMetadata(FIMD_EXIF_MAIN, bmp, "Orientation", &tag) && tag)
				return *static_cast<const uint16_t*>(FreeImage_GetTagValue(tag));
			return uint16_t{ 1 };
		};

	bool mustFlip = false;

	switch (fif)
	{
	case FIF_TARGA:
	{
		FILE* f = fopen(pathStr.c_str(), "rb");
		if (f)
		{
			fseek(f, 17, SEEK_SET);
			uint8_t desc = 0; fread(&desc, 1, 1, f); fclose(f);
			bool originTopLeft = (desc & 0x20) != 0;
			mustFlip = originTopLeft;
		}
		break;
	}
	case FIF_JPEG:
	case FIF_TIFF:
	{
		uint16_t orient = exif_orientation();
		mustFlip = (orient == 3 || orient == 4 || orient == 5 ||
			orient == 6 || orient == 7 || orient == 8);
		break;
	}
	case FIF_EXR:
	{
		try
		{
			Imf::InputFile file(pathStr.c_str());
			const Imf::Header& header = file.header();

			if (const Imf::StringAttribute* attr = header.findTypedAttribute<Imf::StringAttribute>("lineOrder"))
			{
				const std::string& lo = attr->value();
				mustFlip = (lo == "INCREASING_Y");
			}
		}
		catch (const std::exception& e)
		{
			if (logError)
				spdlog::error("LoadImage: EXR metadata read failed: {}", e.what());
		}
		break;
	}
	default:
		mustFlip = false;
		break;
	}

	if (mustFlip)
		FreeImage_FlipVertical(bmp);


	const FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bmp);
	const uint32_t width = FreeImage_GetWidth(bmp);
	const uint32_t height = FreeImage_GetHeight(bmp);

	int32_t internalFormat = 0;
	int32_t externalFormat = 0;
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
			const unsigned bpp = FreeImage_GetBPP(bmp);
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
				FIBITMAP* converted = FreeImage_ConvertTo32Bits(bmp);
				if (!converted)
				{
					FreeImage_Unload(bmp);
					spdlog::error("LoadImage: Failed to convert image to 32-bit '{}'", pathStr);
					return nullptr;
				}
				FreeImage_Unload(bmp);
				bmp = converted;

				myFormat = ImageFormat::U8_RGBA;
				externalFormat = GL_BGRA;
				dataType = GL_UNSIGNED_BYTE;
				internalFormat = (colorSpace == ColorSpace::SRGB) ? GL_SRGB8_ALPHA8 : GL_RGBA8;
				break;
			}

			case 48:
			{
				// RGB16 ? RGB8 (24bit), následnì pøevést na RGBA8 (32bit)
				FIBITMAP* temp24 = FreeImage_ConvertTo24Bits(bmp);
				FreeImage_Unload(bmp);
				if (!temp24)
				{
					spdlog::error("LoadImage: Failed to convert 48bpp to 24bpp '{}'", pathStr);
					return nullptr;
				}
				bmp = FreeImage_ConvertTo32Bits(temp24);
				FreeImage_Unload(temp24);
				if (!bmp)
				{
					spdlog::error("LoadImage: Failed to convert 24bpp to 32bpp '{}'", pathStr);
					return nullptr;
				}

				myFormat = ImageFormat::U8_RGBA;
				externalFormat = GL_BGRA;
				dataType = GL_UNSIGNED_BYTE;
				internalFormat = (colorSpace == ColorSpace::SRGB) ? GL_SRGB8_ALPHA8 : GL_RGBA8;
				break;
			}

			case 64:
			{
				// RGBA16 ? RGBA8 (32bit)
				FIBITMAP* converted = FreeImage_ConvertTo32Bits(bmp);
				FreeImage_Unload(bmp);
				if (!converted)
				{
					spdlog::error("LoadImage: Failed to convert 64bpp to 32bpp '{}'", pathStr);
					return nullptr;
				}
				bmp = converted;

				myFormat = ImageFormat::U8_RGBA;
				externalFormat = GL_BGRA;
				dataType = GL_UNSIGNED_BYTE;
				internalFormat = (colorSpace == ColorSpace::SRGB) ? GL_SRGB8_ALPHA8 : GL_RGBA8;
				break;
			}

			default:
				FreeImage_Unload(bmp);
				spdlog::error("LoadImage: Unsupported BPP {} in '{}'", bpp, pathStr);
				return nullptr;
			}

			break;
		}
	}

	return std::make_unique<GLImage>(bmp, width, height, dataType, internalFormat, externalFormat, myFormat);
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