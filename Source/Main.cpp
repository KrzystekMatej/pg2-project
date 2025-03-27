#include <cassert>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <iostream>

#include "Core/Application.h"


int main()
{
	Application* app = Application::CreateInstance();

	assert(app && "Application is null!");

	if (app->Init())
	{
		app->PrintInfo();
		app->Run();
	}
	else
	{
		spdlog::critical("Application failed to initialize!");
	}

	delete app;
}