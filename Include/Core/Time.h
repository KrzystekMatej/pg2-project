#pragma once
#include <GLFW/glfw3.h>

class Time
{
public:
    static void Update()
    {
        double currentTime = glfwGetTime();
        s_DeltaTime = static_cast<float>(currentTime - s_LastFrameTime);
        s_LastFrameTime = currentTime;
    }

    static float GetDeltaTime() { return s_DeltaTime; }
    static float GetElapsedTime() { return static_cast<float>(glfwGetTime()); }

private:
    static inline double s_LastFrameTime = 0.0;
    static inline float s_DeltaTime = 0.0f;
};
