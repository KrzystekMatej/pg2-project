#include "Application/Window.h"

Window::Window(GLFWwindow* window, int width, int height, const char* title)
	: m_Window(window), m_Width(width), m_Height(height), m_Title(title)
{
	glfwGetFramebufferSize(window, &m_Width, &m_Height);
	glViewport(0, 0, m_Width, m_Height);
}

std::unique_ptr<Window> Window::CreateInstance(int width, int height, const char* title)
{
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(window, cursor);
	if (window) return std::unique_ptr<Window>(new Window(window, width, height, title));
	else return nullptr;
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
}

int Window::GetWidth() const
{
	return m_Width;
}

int Window::GetHeight() const
{
	return m_Height;
}

const char* Window::GetTitle() const
{
	return m_Title;
}

void Window::MakeContext(int interval) const
{
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(interval);
}

bool Window::IsOpen() const
{
	return !glfwWindowShouldClose(m_Window);
}

void Window::SwapBuffers() const
{
	glfwSwapBuffers(m_Window);
}

void Window::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::SetCursorPosition(int x, int y) const
{
	glfwSetCursorPos(m_Window, x, y);
}

void Window::SetCallbacks() const
{
	glfwSetKeyCallback(m_Window, InputHandler::KeyCallBack);
	glfwSetWindowFocusCallback(m_Window, InputHandler::WindowFocusCallback);
	glfwSetWindowIconifyCallback(m_Window, InputHandler::WindowIconifyCallback);
	glfwSetWindowSizeCallback(m_Window, InputHandler::WindowSizeCallback);
	glfwSetCursorPosCallback(m_Window, InputHandler::CursorCallback);
	glfwSetMouseButtonCallback(m_Window, InputHandler::ButtonCallback);
}

glm::vec2 Window::GetCursorPosition() const
{
	double xpos, ypos;
	glfwGetCursorPos(m_Window, &xpos, &ypos);
	return glm::vec2(xpos, ypos);
}
