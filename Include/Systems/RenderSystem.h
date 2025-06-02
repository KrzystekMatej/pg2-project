#pragma once
#include "Assets/AssetManager.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Renderer/Buffers/ShaderStorageBuffer.h"
#include "Assets/TextureRegistry.h"

class RenderSystem
{
public:
	void Initialize(const AssetManager& assetManager);
    void Draw(Scene& scene, float aspectRatio);
private:
	const Texture* m_BackgroundTexture;
	const ShaderProgram* m_BackgroundShader;
};
