#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Events/InputHandler.h"

class Window
{
public:
	static std::unique_ptr<Window> CreateInstance(int width, int height, const char* title);
	~Window();

	int GetWidth() const;
	int GetHeight() const;
	const char* GetTitle() const;

	void MakeContext(int interval) const;
	bool IsOpen() const;
	void SwapBuffers() const;
	void Clear() const;
	void SetCallbacks() const;
	glm::vec2 GetCursorPosition() const;
	void SetCursorPosition(int x, int y) const;
private:
	Window(GLFWwindow* window, int width, int height, const char* title);

	GLFWwindow* m_Window;
	int m_Width;
	int m_Height;
	const char* m_Title;
};