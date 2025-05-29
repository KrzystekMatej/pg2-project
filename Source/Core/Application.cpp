#include <glad/gl.h>
#include "Core/Application.h"

#include <iostream>
#include <spdlog/spdlog.h>

#include "Core/Time.h"
#include "Scene/SceneLoader.h"
#include "Assets/MeshRegistry.h"

Application* Application::CreateInstance(const std::filesystem::path& projectConfigPath)
{
	if (!glfwInit())
	{
		spdlog::critical("Could not start GLFW3!");
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	return new Application(projectConfigPath);
}

Application::Application(const std::filesystem::path& projectConfigPath)
	: m_Project(Project::Load(projectConfigPath)) {}

Application::~Application()
{
	this->Terminate();
}

bool Application::Initialize()
{
	if (PrepareWindow(1920, 1080, m_Project.GetConfig().Name.c_str()) && m_Window.MakeContext(1))
	{
		m_Window.InitializeGL();
		m_Window.SetCallbacks();
		m_AssetManager.Initialize();
		if (!glfwExtensionSupported("GL_ARB_bindless_texture"))
		{
			spdlog::critical("GL_ARB_bindless_texture not supported!");
		}
		SceneLoader::Load(&m_ActiveScene, "", m_Project, &m_Window, m_AssetManager);
		m_RenderSystem.Initialize(m_AssetManager);
		return true;
	}
	return false;
}

bool Application::PrepareWindow(int width, int height, const char* title)
{
	GLFWwindow* glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!glfwWindow)
	{
		spdlog::critical("Failed to create GLFW window!");
		return false;
	}

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);

	if (!cursor) spdlog::error("Failed to create cursor, using default system cursor.");
	else glfwSetCursor(glfwWindow, cursor);

	m_Window.Create(glfwWindow, width, height, title);
	return true;
}



void Application::Run()
{
	while (m_Window.IsOpen())
	{
		Time::Update();
		glfwPollEvents();

		m_AISystem.Update(m_ActiveScene);

		m_Window.Clear();
		m_RenderSystem.Draw(m_ActiveScene, m_Window.GetAspectRatio());
		m_Window.SwapBuffers();
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

void Application::Terminate() const
{
	glfwTerminate();
}

