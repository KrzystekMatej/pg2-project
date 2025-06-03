#pragma once
#include "Assets/AssetManager.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Renderer/Buffers/ShaderStorageBuffer.h"
#include "Assets/TextureRegistry.h"
#include "ECS/Components/PointLight.h"

class Window;

class RenderSystem
{
public:
	void Initialize(const AssetManager& assetManager);
	void Draw(Scene& scene, const Window& window);
private:
	const Texture* m_BackgroundTexture;
	const ShaderProgram* m_BackgroundShader;
	const ShaderProgram* m_ShadowShader;
};
