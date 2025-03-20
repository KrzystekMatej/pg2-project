#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <glm/vec2.hpp>
#include "EventSystem/Events/FrameBufferSizeEvent.h"

class Window
{
public:
    static std::unique_ptr<Window> CreateInstance(int width, int height, const char* title);
    ~Window();

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    const char* GetTitle() const { return m_Title; }

    void MakeContext(int interval) const;
    bool IsOpen() const { return !glfwWindowShouldClose(m_Window); }
    void SwapBuffers() const { glfwSwapBuffers(m_Window); }
    void Clear() const;

    glm::vec2 GetCursorPosition() const;
    void SetCursorPosition(int x, int y) const { glfwSetCursorPos(m_Window, x, y); }
    void SetCallbacks() const;
private:
    Window(GLFWwindow* window, int width, int height, const char* title);
    void OnFrameBufferChanged(const Event& event);

    GLFWwindow* m_Window;
    int m_Width;
    int m_Height;
    const char* m_Title;
};
