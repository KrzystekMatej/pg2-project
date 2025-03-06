#pragma once
#include "Window.h"

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

	void InitGlew() const;
	void TerminateApplication() const;

	std::unique_ptr<Window> m_Window;
};

