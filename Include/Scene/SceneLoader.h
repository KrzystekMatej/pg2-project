#pragma once
#include <filesystem>
#include "ECS/Entity.h"
#include "Assets/AssetManager.h"
#include "Core/Window.h"
#include "Renderer/Material/Shaders/ShaderProgram.h"
#include "Renderer/Material/Shaders/ShaderPipeline.h"

class SceneLoader
{
public:
	static void Load(Scene* scene, const std::filesystem::path& filePath, const Project& project, const Window* window, const AssetManager& assetManager);
private:
	static void CreateTestSpheres(Scene* scene, const Project& project, const Window* window, const AssetManager& assetManager);
};
