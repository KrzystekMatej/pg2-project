#include "Core/Window.h"

#include <iostream>
#include <spdlog/spdlog.h>

#include "EventSystem/GLFWCallbacks.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/Event.h"
#include "EventSystem/Events/KeyEvent.h"

Window::~Window()
{
    glfwDestroyWindow(m_Window);
}

bool Window::Create(GLFWwindow* window, int width, int height, const char* title)
{
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }

    if (!window)
    {
        spdlog::critical("Window::Create received null GLFWwindow!");
        return false;
    }

    m_Window = window;
    m_Width = width;
    m_Height = height;
    m_Title = title;

    glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);

    EventDispatcher::AddListener(Event::Type::FrameBufferSize,
        [this](const Event& e) { OnFrameBufferSizeChanged(e); });
    EventDispatcher::AddListener(Event::Type::Key,
        [this](const Event& e) { OnKeyPressed(e); });
    EventDispatcher::AddListener(Event::Type::CursorPosition,
        [this](const Event& e) { OnCursorPositionChanged(e); });

    return true;
}

bool Window::MakeContext(int interval) const
{
    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(interval);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) && !gladLoadGL())
    {
        spdlog::critical("Failed to initialize GLAD!");
        return false;
    }
    return true;
}

void Window::InitializeGL()
{
    glClipControl(GL_UPPER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_MULTISAMPLE);
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glViewport(0, 0, m_Width, m_Height);
}

void Window::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::SetCallbacks() const
{
    SetGLFWCallbacks(m_Window);
}

void Window::OnFrameBufferSizeChanged(const Event& event)
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

void Window::OnKeyPressed(const Event& event) const
{
    const auto& keyEvent = static_cast<const KeyEvent&>(event);

    if (keyEvent.GetKey() == GLFW_KEY_ESCAPE && keyEvent.GetAction() == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_Window, GL_TRUE);
    }
}

void Window::OnCursorPositionChanged(const Event& event) const
{
    SetCursorPosition(m_Width / 2, m_Height / 2);
}
