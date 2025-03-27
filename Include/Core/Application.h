#pragma once
#include "Systems/RenderSystem.h"
#include "Window.h"
#include "Assets/AssetManager.h"
#include "Scene/Scene.h"
#include "Systems/AISystem.h"

class Application
{
public:
	static Application* CreateInstance();
	~Application();

	bool Init();
	void Run();
	void PrintInfo() const;
private:
	Application();
	void TerminateApplication() const;

	std::unique_ptr<Window> m_Window;
	Scene m_ActiveScene;
	AISystem m_AISystem;
	RenderSystem m_RenderSystem;
	AssetManager m_AssetManager;
};

