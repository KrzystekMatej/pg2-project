#pragma once
#include <memory>

class Texture
{
public:
	Texture(uint32_t id, uint64_t handle, uint32_t width, uint32_t height)
		: m_Id(id), m_Handle(handle), m_Width(width), m_Height(height) {}

	~Texture();

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	uint64_t GetBindlessHandle() const { return m_Handle; }
private:
	uint32_t m_Id = 0;
	uint64_t m_Handle = 0;

	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
};
