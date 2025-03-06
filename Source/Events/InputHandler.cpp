#include "Events/InputHandler.h"

InputHandler::InputHandler()
{
	glfwSetErrorCallback(ErrorCallback);
}

InputHandler& InputHandler::Instance()
{
	static InputHandler instance;
	return instance;
}

void InputHandler::PollEvents()
{
	glfwPollEvents();
}

void InputHandler::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void InputHandler::ErrorCallback(int error, const char* description)
{
	 Logger::LogLevel::ERROR(std::format("{}: {}!", error, description));
}

void InputHandler::WindowFocusCallback(GLFWwindow* window, int focused)
{
}

void InputHandler::WindowIconifyCallback(GLFWwindow* window, int iconified)
{
}

void InputHandler::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
}

void InputHandler::CursorCallback(GLFWwindow* window, double x, double y)
{
}

void InputHandler::ButtonCallback(GLFWwindow* window, int button, int action, int mode)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
}