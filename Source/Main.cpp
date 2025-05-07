#include <cassert>
#include <spdlog/spdlog.h>
#include <filesystem>

#include "Core/Application.h"


int main()
{
	Application* app = Application::CreateInstance("../../config.yaml");

	assert(app && "Application is null!");

	if (app->Initialize())
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