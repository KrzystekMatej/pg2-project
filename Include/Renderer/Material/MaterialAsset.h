#pragma once
#include <string>
#include <array>
#include <glm/glm.hpp>
#include "Assets/TextureRegistry.h"

enum class Map : uint8_t { Diffuse, Specular, Glossiness, RMA, Normal, Opacity, Emission, MapCount };
#define IOR_AIR 1.000293f
#define IOR_WATER 1.33f
#define IOR_GLASS 1.5f

#pragma pack(push, 1)
struct GLMaterial
{
	uint64_t textureHandles[static_cast<size_t>(Map::MapCount)];
};
#pragma pack(pop)

namespace MaterialDefaults
{
	inline const glm::vec3 Diffuse = glm::vec3(1.0f);
	inline const glm::vec3 Specular = glm::vec3(0.0f);
	inline const glm::vec3 Glossiness = glm::vec3(1.0f);
	inline const glm::vec3 RMA = glm::vec3(1.0f, 0.0f, 1.0f);
	inline const glm::vec3 Normal = glm::vec3(0.5f, 0.5f, 1.0f);
	inline const glm::vec3 Opacity = glm::vec3(1.0f);
	inline const glm::vec3 Emission = glm::vec3(0.0f);

	inline const glm::vec3 Fallbacks[static_cast<size_t>(Map::MapCount)] =
	{
		Diffuse,
		Specular,
		Glossiness,
		RMA,
		Normal,
		Opacity,
		Emission,
	};
}



struct MaterialAsset
{
	std::string Name;

	float Reflectivity = 0.04f;
	float Ior = IOR_AIR;

	void SetTexture(Map type, const Texture* texture)
	{
		m_Textures[static_cast<size_t>(type)] = texture;
	}

	const Texture* GetTexture(Map type) const
	{
		return m_Textures[static_cast<size_t>(type)];
	}

	void SetFallback(Map type, const glm::vec3& value)
	{
		m_fallbackValues[static_cast<size_t>(type)] = value;
	}

	glm::vec3 GetFallback(Map type) const
	{
		return m_fallbackValues[static_cast<size_t>(type)];
	}

	GLMaterial ToGLMaterial() const
	{
		GLMaterial glMat{};

		for (size_t i = 0; i < static_cast<size_t>(Map::MapCount); ++i)
		{
			glMat.textureHandles[i] = GetTexture(static_cast<Map>(i))->GetBindlessHandle();
		}

		return glMat;
	}

private:
	std::array<const Texture*, static_cast<size_t>(Map::MapCount)> m_Textures{ nullptr };

	std::array<glm::vec3, static_cast<size_t>(Map::MapCount)> m_fallbackValues
	{
		MaterialDefaults::Diffuse,
		MaterialDefaults::Specular,
		MaterialDefaults::Glossiness,
		MaterialDefaults::RMA,
		MaterialDefaults::Normal,
		MaterialDefaults::Opacity,
		MaterialDefaults::Emission
	};
};
