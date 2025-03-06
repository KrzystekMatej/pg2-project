#include "Application/Application.h"


Application::Application() {}

Application* Application::CreateInstance()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: Could not start GLFW3!\n");
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
	m_Window = Window::CreateInstance(1024, 768, "PG-2");

	if (m_Window)
	{
		m_Window->MakeContext(1);
		InitGlew();

		m_Window->SetCallbacks();
		return true;
	}
	return false;
}


void Application::Run()
{
	double lastTime = 0;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	while (m_Window->IsOpen())
	{
		double now = glfwGetTime();
		double dt = now - lastTime;
		lastTime = now;
		m_Window->Clear();
		m_Window->SwapBuffers();
	}
}

void Application::PrintInfo() const
{
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Vendor " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	std::cout << "Using GLFW " << major << "." << minor << "." << revision << std::endl;
}

void Application::InitGlew() const
{
	glewExperimental = GL_TRUE;
	glewInit();
}

void Application::TerminateApplication() const
{
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

