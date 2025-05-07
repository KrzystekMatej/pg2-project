#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera
{
    Camera(float fovY, float nearPlane, float farPlane)
		: FovY(fovY), NearPlane(nearPlane), FarPlane(farPlane) {}

    glm::mat4 GetProjectionMatrix(float aspectRatio) const
    {
        float f = 1.0f / std::tan(FovY / 2.0f);
        float a = (NearPlane + FarPlane) / (NearPlane - FarPlane);
        float b = (2.0f * NearPlane * FarPlane) / (NearPlane - FarPlane);

        float sign = -1.0f;

        return glm::mat4
    	{
            f / aspectRatio,    0.0f,       0.0f,   0.0f,
            0.0f,               sign * f,   0.0f,   0.0f,
            0.0f,               0.0f,       a,      -1.0f,
            0.0f,               0.0f,       b,      0.0f
        };
    }

    float FovY;
    float NearPlane;
    float FarPlane;
};