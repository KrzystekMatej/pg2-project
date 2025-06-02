#pragma once
#include <glm/glm.hpp>
#include <filesystem>
#include "Renderer/Buffers/Mesh/VertexArray.h"
#include "Renderer/Buffers/Mesh/IndexBuffer.h"
#include "Renderer/Material/Shaders/ShaderProgram.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/PointLight.h"
#include "ECS/Components/Material.h"
#include "ECS/Components/Mesh.h"
#include "DrawContext.h"
#include "Assets/TextureRegistry.h"
#include "Assets/ShaderRegistry.h"

class Renderer
{
public:
	static void DrawPass(entt::registry& registry, const DrawContext& drawCtx);
	static void DiffuseBackgroundPass(const ShaderProgram& backgroundShader, const Texture& backgroundTexture, const glm::mat4 projection, const glm::mat4& view);
	static const Texture* IrradianceMapPass(std::filesystem::path directoryPath, TextureRegistry* textureRegistry, ShaderRegistry* shaderRegistry, const Project& project);
};