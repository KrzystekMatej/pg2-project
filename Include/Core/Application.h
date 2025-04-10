#pragma once
#include "Systems/RenderSystem.h"
#include "Window.h"
#include "Assets/AssetManager.h"
#include "Scene/Scene.h"
#include "Systems/AISystem.h"
#include "Assets/ShaderRegistry.h"
#include "Assets/MeshRegistry.h"
#include "Project/Project.h"

class Application
{
public:
	static Application* CreateInstance(const std::filesystem::path& projectConfigPath);
	Application(const std::filesystem::path& projectConfigPath) : m_Project(Project::Load(projectConfigPath))
	{
		m_AssetManager.AddRegistry<ShaderRegistry>();
		m_AssetManager.AddRegistry<MeshRegistry>();
	}
	~Application();

	bool Init();
	void Run();
	void PrintInfo() const;
private:
	bool CreateWindow(int width, int height, const char* title);
	void TerminateApplication() const;

	Window m_Window;
	Project m_Project;
	Scene m_ActiveScene;
	AISystem m_AISystem;
	RenderSystem m_RenderSystem;
	AssetManager m_AssetManager;
};

