#include "Renderer/VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_VAO);
	Bind();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	uint32_t offset = 0;
	for (std::size_t i = 0; i < elements.size(); i++)
	{
		const VertexBufferElement& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_VAO);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}