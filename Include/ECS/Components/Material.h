#pragma once
#include "Renderer/ShaderProgram.h"

struct Material
{
	Material(const ShaderProgram* shaderProgram) : ShaderProgram(shaderProgram) {}

	const ShaderProgram* ShaderProgram;
};