#pragma once
#include <string>
#include <array>
#include <glm/glm.hpp>
#include "Texture.h"

enum class Map : uint8_t { Diffuse, Specular, Glossiness, RMA, Normal, Opacity, Emission, MapCount };
#define IOR_AIR 1.000293f
#define IOR_WATER 1.33f
#define IOR_GLASS 1.5f
#define AO_DEFAULT 1.0f

#pragma pack(push, 1)
struct GLMaterial
{
	uint64_t textureHandles[static_cast<size_t>(Map::MapCount)];
};
#pragma pack(pop)

struct MaterialAsset
{
	std::string Name;

	float Shininess = 32.0f;
	float Roughness = 1.0f;
	float Metallic = 0.0f;
	float Reflectivity = 0.04f;
	float Ior = IOR_AIR;

	glm::vec3 Emission = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Attenuation = { 0.0f, 0.0f, 0.0f };

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

	const glm::vec3& GetFallback(Map type) const
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
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f),
		glm::vec3(0.0f)
	};
};
