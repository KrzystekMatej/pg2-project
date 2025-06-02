#pragma once
#include "Systems/RenderSystem.h"
#include "Window.h"
#include "Assets/AssetManager.h"
#include "Assets/MaterialRegistry.h"
#include "Scene/Scene.h"
#include "Systems/AISystem.h"
#include "Project/Project.h"
#include "Config.h"

class Application
{
public:
	static Application* CreateInstance(const std::filesystem::path& projectConfigPath);
	Application(const std::filesystem::path& projectConfigPath);
	~Application();

	bool Initialize();

	void Run();
	void PrintInfo() const;
private:
	bool PrepareWindow(int width, int height, const char* title);
	void Terminate() const;
#if DEBUG_LOGGING == 1
	void EnableOpenGLDebug() const;
#endif

	Window m_Window;
	Project m_Project;
	Scene m_ActiveScene;
	AISystem m_AISystem;
	RenderSystem m_RenderSystem;
	AssetManager m_AssetManager;
};

