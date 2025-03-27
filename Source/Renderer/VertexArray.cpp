#include "Renderer/VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_VAO);
	Bind();
}

VertexArray::VertexArray(VertexArray&& other) noexcept
	: m_VAO(other.m_VAO), m_VertexBuffers(std::move(other.m_VertexBuffers))
{
	other.m_VAO = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this != &other)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = other.m_VAO;
		m_VertexBuffers = std::move(other.m_VertexBuffers);
		other.m_VAO = 0;
	}
	return *this;
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::AddBuffer(VertexBuffer&& vertexBuffer, const VertexBufferLayout& layout)
{
	Bind();
	vertexBuffer.Bind();
	const auto& elements = layout.GetElements();
	uint32_t offset = 0;
	for (std::size_t i = 0; i < elements.size(); i++)
	{
		const VertexBufferElement& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	m_VertexBuffers.push_back(std::move(vertexBuffer));
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_VAO);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}