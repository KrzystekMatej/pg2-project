#pragma once
#include <string>
#include <array>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"

enum class Map : char { kDiffuse = 0, kSpecular, kGlossiness, kBase, kRoughness, kMetallic, kRMA, kNormal, kOpacity, kEmission, kMapsCount };
#define IOR_AIR 1.000293f

struct MaterialAsset
{
	std::string Name;

	const ShaderProgram* ShaderProgram = nullptr;

	float Shininess = 32.0f;
	float Roughness = 1.0f;
	float Metallic = 0.0f;
	float Reflectivity = 0.0f;
	float Ior = IOR_AIR;

	glm::vec3 Emission = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Attenuation = { 0.0f, 0.0f, 0.0f };

	void SetTexture(Map type, std::shared_ptr<Texture> texture)
	{
		m_Textures[static_cast<size_t>(type)] = std::move(texture);
	}

	std::shared_ptr<Texture> GetTexture(Map type) const
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

	void BindTextures() const
	{
		for (size_t i = 0; i < m_Textures.size(); ++i)
		{
			if (m_Textures[i]) m_Textures[i]->Bind(GL_TEXTURE0 + static_cast<uint32_t>(i));
		}
	}

private:
	std::array<std::shared_ptr<Texture>, static_cast<size_t>(Map::kMapsCount)> m_Textures{ nullptr };
	std::array<glm::vec3, static_cast<size_t>(Map::kMapsCount)> m_fallbackValues{};
};
