#include "Core/Window.h"

#include <iostream>
#include <spdlog/spdlog.h>

#include "EventSystem/GLFWCallbacks.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/Event.h"

Window::Window(GLFWwindow* window, int width, int height, const char* title)
    : m_Window(window), m_Width(width), m_Height(height), m_Title(title)
{
    glfwGetFramebufferSize(window, &m_Width, &m_Height);
    glViewport(0, 0, m_Width, m_Height);
    EventDispatcher::AddListener(Event::Type::FrameBufferSize, [this](const Event& event) { OnFrameBufferChanged(event); });
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

void Window::OnFrameBufferChanged(const Event& event)
{
    const auto& fbEvent = static_cast<const FrameBufferSizeEvent&>(event);
    m_Width = fbEvent.GetWidth();
    m_Height = fbEvent.GetHeight();
    glViewport(0, 0, m_Width, m_Height);

    spdlog::info("Framebuffer resized to: {}x{}", m_Width, m_Height);
}


glm::vec2 Window::GetCursorPosition() const
{
    double xpos, ypos;
    glfwGetCursorPos(m_Window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}
