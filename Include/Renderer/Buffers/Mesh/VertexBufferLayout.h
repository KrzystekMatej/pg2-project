#pragma once
#include <vector>
#include <glad/gl.h>

struct VertexBufferElement
{
	uint32_t type;
	uint32_t count;
	uint32_t normalized;

	static uint32_t GetSizeOfType(uint32_t type)
	{
		switch (type)
		{
			case GL_FLOAT:
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
			default: return 0;
		}
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() : m_Stride(0) {}

	template<typename T>
	void Push(uint32_t count) {}

	template<>
	void Push<float>(uint32_t count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<uint32_t>(uint32_t count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<uint8_t>(uint32_t count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	uint32_t GetStride() const { return m_Stride; }
	const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
private:
	std::vector<VertexBufferElement> m_Elements;
	uint32_t m_Stride;
};