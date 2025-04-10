#pragma once
#include "Renderer/MaterialAsset.h"

struct Material
{
	Material(const ShaderProgram* shaderProgram) : ShaderProgram(shaderProgram) {}

	const ShaderProgram* ShaderProgram;
	const MaterialAsset* MaterialAsset;
};