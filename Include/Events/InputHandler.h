#pragma once
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>
#include "Logger.h"

class InputHandler
{
public:
	static InputHandler& Instance();

	void PollEvents();

	static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ErrorCallback(int error, const char* description);
	static void WindowFocusCallback(GLFWwindow* window, int focused);
	static void WindowIconifyCallback(GLFWwindow* window, int iconified);
	static void WindowSizeCallback(GLFWwindow* window, int width, int height);
	static void CursorCallback(GLFWwindow* window, double x, double y);
	static void ButtonCallback(GLFWwindow* window, int button, int action, int mode);
private:
	InputHandler();
};