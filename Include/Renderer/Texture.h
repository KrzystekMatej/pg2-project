#pragma once
#include <memory>

enum class TextureFormat
{
	RGB8,
	RGBA8,
	RGB16F,
	RGB32F
};

class Texture
{
public:
	Texture(uint32_t id, uint64_t handle, uint32_t width, uint32_t height, TextureFormat format)
		: m_Id(id), m_Handle(handle), m_Width(width), m_Height(height), m_Format(format) {
	}

	~Texture();

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	TextureFormat GetFormat() const { return m_Format; }
	uint64_t GetBindlessHandle() const { return m_Handle; }
private:
	uint32_t m_Id = 0;
	uint64_t m_Handle = 0;

	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
	TextureFormat m_Format = TextureFormat::RGB8;
};
