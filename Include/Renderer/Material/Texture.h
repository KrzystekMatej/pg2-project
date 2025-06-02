#pragma once
#include <memory>
#include "Config.h"

class TextureRegistry;

class Texture
{
public:
	Texture(uint32_t id, uint32_t target, uint64_t handle, uint32_t width, uint32_t height)
		: m_Id(id), m_Target(target), m_Handle(handle), m_Width(width), m_Height(height) {}
	~Texture();

	uint32_t GetId() const { return m_Id; }
	uint32_t GetTarget() const { return m_Target; }
	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	void Bind() const;
	void Unbind() const;
	static void Activate(uint32_t index = 0);
	uint64_t GetBindlessHandle() const { return m_Handle; }
private:
	friend TextureRegistry;
	uint32_t m_Id = 0;
	uint64_t m_Handle = 0;
	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
	uint32_t m_Target;
};
