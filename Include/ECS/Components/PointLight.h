#pragma once
#include <glm/glm.hpp>
#include "Renderer/Material/Texture.h"

struct PointLight
{
	PointLight(glm::vec3 color, float farPlane) : Color(color), FarPlane(farPlane) {}

	glm::vec3 Color;
	float FarPlane;

	static constexpr uint32_t s_MaxLightCount = 10;
	static inline std::unique_ptr<Texture> s_DepthMaps[s_MaxLightCount] = {};
};