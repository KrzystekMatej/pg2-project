#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

class Renderer
{
public:
	Renderer(uint32_t renderMode)
		: m_RenderMode(renderMode) {}

	void Draw(const ShaderProgram* shaderProgram, const VertexArray& vertexArray, const IndexBuffer& indexBuffer) const;
private:
	uint32_t m_RenderMode;
};