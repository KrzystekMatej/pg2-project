#pragma once
#include <vector>
#include "Renderer/Material/Shaders/ShaderPipeline.h"

struct Material
{
	Material() = default;
	std::vector<ShaderPipeline> pipelines;
};
