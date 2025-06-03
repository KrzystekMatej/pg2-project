#include "Renderer/Buffers/Mesh/VertexArray.h"

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
		if (m_Id) glDeleteVertexArrays(1, &m_Id);
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

void VertexArray::AddBuffer(VertexBuffer&& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();

    const auto& elements = layout.GetElements();
    uint32_t offset = 0;

    for (GLuint i = 0; i < elements.size(); ++i)
    {
        const auto& e = elements[i];

        if (e.type == GL_UNSIGNED_INT || e.type == GL_INT)
        {
            glVertexAttribIPointer(i, e.count, e.type, layout.GetStride(), reinterpret_cast<void*>(static_cast<uintptr_t>(offset)));
        }
        else
        {
            glVertexAttribPointer(i, e.count, e.type, e.normalized, layout.GetStride(), reinterpret_cast<void*>(static_cast<uintptr_t>(offset)));
        }
        glEnableVertexAttribArray(i);
        offset += e.count * VertexBufferElement::GetSizeOfType(e.type);
    }

    m_VertexBuffers.push_back(std::move(vb));
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_Id);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}