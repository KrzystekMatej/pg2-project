#include "Renderer/Renderer.h"

void Renderer::Draw(const ShaderProgram* shaderProgram, const VertexArray& vertexArray, const IndexBuffer& indexBuffer) const
{
	if (shaderProgram)
	{
		vertexArray.Bind();
		glDrawElements(m_RenderMode, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
