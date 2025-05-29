#pragma once
#include <glm/glm.hpp>
#include "Buffers/Mesh/VertexArray.h"
#include "Buffers/Mesh/IndexBuffer.h"
#include "Material/Shaders/ShaderProgram.h"

class Renderer
{
public:
	Renderer(uint32_t renderMode)
		: m_RenderMode(renderMode) {}

	void Draw(const ShaderProgram* shaderProgram, const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const glm::mat4& pv, const glm::mat4& model, const glm::mat3& normal) const;
private:
	uint32_t m_RenderMode;
};