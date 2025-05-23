#pragma once
#include <filesystem>
#include "ECS/Entity.h"
#include "Assets/AssetManager.h"
#include "Core/Window.h"
#include "Renderer/ShaderProgram.h"

class SceneLoader
{
public:
	static void Load(Scene* scene, const std::filesystem::path& filePath, const Project& project, const Window* window, const AssetManager& assetManager);
	static void LoadObj(Scene* scene, const ShaderProgram* program, const std::filesystem::path& filePath, const AssetManager& assetManager);

private:
};
