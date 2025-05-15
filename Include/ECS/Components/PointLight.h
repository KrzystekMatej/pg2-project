#pragma once
#include <glm/glm.hpp>

struct PointLight
{
	PointLight(glm::vec3 color) : Color(color) {}

	glm::vec3 Color;

	static constexpr uint32_t s_MaxLightCount = 10;
};