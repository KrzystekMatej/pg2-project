#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>
#include <memory>

enum class TextureFormat
{
	RGB8,
	RGBA8,
	RGB16F,
	RGB32F
};

enum class TextureType
{
	Color,
	Normal,
	Roughness,
	Metallic,
	Emission,
	Generic
};

class Texture
{
public:
	Texture(uint32_t id, uint32_t width, uint32_t height, TextureFormat format, TextureType type)
		: m_Id(id), m_Width(width), m_Height(height), m_Format(format), m_Type(type) {}
	~Texture();

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	TextureFormat GetFormat() const { return m_Format; }
	TextureType GetType() const { return m_Type; }

	void Bind(uint32_t unit = GL_TEXTURE0) const;

	static std::shared_ptr<Texture> CreateFromFile(const std::string& path, TextureType type = TextureType::Generic);
	static std::shared_ptr<Texture> CreateFromMipChain(const std::vector<std::string>& paths, TextureFormat format = TextureFormat::RGB32F, TextureType type = TextureType::Generic);
private:
	uint32_t m_Id = 0;
	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
	TextureFormat m_Format = TextureFormat::RGB8;
	TextureType m_Type = TextureType::Generic;
};
