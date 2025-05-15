#pragma once
#include "Assets/AssetManager.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Renderer/ShaderStorageBuffer.h"

class RenderSystem
{
public:
	void Initialize(const AssetManager& assetManager) const;
    void Draw(Scene& scene, float aspectRatio) const;
};
