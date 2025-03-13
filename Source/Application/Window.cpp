#include "Application/Window.h"
#include "EventSystem/GLFWCallbacks.h"

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
    return nullptr;
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
}

void Window::MakeContext(int interval) const
{
    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(interval);
}

void Window::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::SetCallbacks() const
{
    SetGLFWCallbacks(m_Window);
}

glm::vec2 Window::GetCursorPosition() const
{
    double xpos, ypos;
    glfwGetCursorPos(m_Window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}
