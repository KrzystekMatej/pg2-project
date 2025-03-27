#include "Renderer/Renderer.h"
#include <glm/gtc/type_ptr.hpp>

void Renderer::Draw(const ShaderProgram* shaderProgram, const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const glm::mat4& pv, const glm::mat4& model) const
{

	shaderProgram->Use();
	shaderProgram->SetUniform(UniformType::Matrix4x4, "modelMatrix", glm::value_ptr(model));
	shaderProgram->SetUniform
	(
		UniformType::Matrix3x3,
		"normalMatrix",
		glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(model))))
	);
	shaderProgram->SetUniform(UniformType::Matrix4x4, "pvmMatrix", glm::value_ptr(pv * model));
	if (shaderProgram)
	{
		vertexArray.Bind();
		glDrawElements(m_RenderMode, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
