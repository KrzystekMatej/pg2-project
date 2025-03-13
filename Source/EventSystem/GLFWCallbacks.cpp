#include "EventSystem/GLFWCallbacks.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/KeyEvent.h"
#include "EventSystem/Events/MouseButtonEvent.h"
#include "EventSystem/Events/CursorPositionEvent.h"
#include "EventSystem/Events/FrameBufferSizeEvent.h"

void SetGLFWCallbacks(GLFWwindow* window)
{
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        KeyEvent event(key, mods);
        EventDispatcher::DispatchEvent(event);
    }
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    FrameBufferSizeEvent event(width, height);
    EventDispatcher::DispatchEvent(event);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_RELEASE)
    {
        MouseButtonEvent event(button, action, mods);
        EventDispatcher::DispatchEvent(event);
    }
}

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    CursorPositionEvent event(xpos, ypos);
    EventDispatcher::DispatchEvent(event);
}
