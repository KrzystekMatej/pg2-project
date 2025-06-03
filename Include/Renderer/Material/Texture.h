#pragma once
#include <memory>
#include "Config.h"

class TextureRegistry;


class Texture
{
public:
	Texture() : m_Id(0), m_Target(0), m_Handle(0), m_Width(0), m_Height(0) {}

	Texture(uint32_t id, uint32_t target, uint64_t handle, uint32_t width, uint32_t height)
		: m_Id(id), m_Target(target), m_Handle(handle), m_Width(width), m_Height(height) {}

	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept
		: m_Id(other.m_Id), m_Target(other.m_Target), m_Handle(other.m_Handle),
		m_Width(other.m_Width), m_Height(other.m_Height)
	{
		other.m_Id = 0;
		other.m_Handle = 0;
		other.m_Width = 0;
		other.m_Height = 0;
	}

	Texture& operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			this->~Texture();
			m_Id = other.m_Id;
			m_Target = other.m_Target;
			m_Handle = other.m_Handle;
			m_Width = other.m_Width;
			m_Height = other.m_Height;
			other.m_Id = 0;
			other.m_Handle = 0;
			other.m_Width = 0;
			other.m_Height = 0;
		}
		return *this;
	}

	uint32_t GetId() const { return m_Id; }
	uint32_t GetTarget() const { return m_Target; }
	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	void Bind() const;
	void Unbind() const;
	static void Activate(uint32_t index = 0);
	uint64_t GetBindlessHandle() const { return m_Handle; }
	void GenerateMipmaps() const;

private:
	friend TextureRegistry;
	uint32_t m_Id = 0;
	uint64_t m_Handle = 0;
	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
	uint32_t m_Target = 0;
};
