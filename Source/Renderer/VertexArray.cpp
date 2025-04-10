#include "Renderer/VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_Id);
	Bind();
}

VertexArray::VertexArray(VertexArray&& other) noexcept
	: m_Id(other.m_Id), m_VertexBuffers(std::move(other.m_VertexBuffers))
{
	other.m_Id = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this != &other)
	{
		glDeleteVertexArrays(1, &m_Id);
		m_Id = other.m_Id;
		m_VertexBuffers = std::move(other.m_VertexBuffers);
		other.m_Id = 0;
	}
	return *this;
}

VertexArray::~VertexArray()
{
	if (m_Id)
	{
		glDeleteVertexArrays(1, &m_Id);
		m_Id = 0;
	}
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
	glBindVertexArray(m_Id);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}