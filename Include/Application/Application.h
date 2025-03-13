#pragma once
#include <glad/glad.h>
#include "Window.h"
#include <iostream>

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
};

