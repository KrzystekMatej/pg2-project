#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <glm/vec2.hpp>
#include "EventSystem/Events/FrameBufferSizeEvent.h"

class Window
{
public:
    Window() : m_Window(nullptr), m_Width(0), m_Height(0), m_Title("") {}
    ~Window();

    bool Create(GLFWwindow* window, int width, int height, const char* title);

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    const char* GetTitle() const { return m_Title; }

    bool MakeContext(int interval) const;
    void InitializeGL();
    bool IsOpen() const { return !glfwWindowShouldClose(m_Window); }
    void SwapBuffers() const { glfwSwapBuffers(m_Window); }
    void Clear() const;

    glm::vec2 GetCursorPosition() const;
    void SetCursorPosition(int x, int y) const { glfwSetCursorPos(m_Window, x, y); }
    void SetCallbacks() const;
private:
    void OnFrameBufferSizeChanged(const Event& event);
    void OnKeyPressed(const Event& event) const;
    void OnCursorPositionChanged(const Event& event) const;

    GLFWwindow* m_Window;
    int m_Width;
    int m_Height;
    const char* m_Title;
};
