#include <glad/glad.h>
#include "Core/Application.h"

#include <iostream>
#include <spdlog/spdlog.h>

#include "Core/Time.h"


Application::Application() {}

Application* Application::CreateInstance()
{
	if (!glfwInit())
	{
		spdlog::critical("Could not start GLFW3!");
		return nullptr;
	}

	return new Application();
}

Application::~Application()
{
	this->TerminateApplication();
}

bool Application::Init()
{
	m_Window = Window::CreateInstance(1280, 720, "PG-2");

	if (m_Window)
	{
		m_Window->MakeContext(1);
		m_Window->SetCallbacks();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			spdlog::critical("Failed to initialize GLAD!");
			return false;
		}

		//glClipControl(GL_UPPER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
		m_ActiveScene.Load(m_AssetManager);
		return true;
	}
	return false;
}


void Application::Run()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	while (m_Window->IsOpen())
	{
		Time::Update();
		glfwPollEvents();

		m_AISystem.Update(m_ActiveScene);

		m_Window->Clear();
		m_RenderSystem.Draw(m_ActiveScene);
		m_Window->SwapBuffers();
	}
}

void Application::PrintInfo() const
{
	spdlog::info("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	spdlog::info("Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	spdlog::info("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	spdlog::info("GLSL: {}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	spdlog::info("GLFW Version: {}.{}.{}", major, minor, revision);
}

void Application::TerminateApplication() const
{
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

