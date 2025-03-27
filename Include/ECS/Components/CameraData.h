#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct CameraData
{
    CameraData(float fov, int width, int heigth, float nearPlane, float farPlane)
		: Fov(fov), Width(width), Height(heigth), NearPlane(nearPlane), FarPlane(farPlane) {}

    glm::mat4 GetProjectionMatrix() const
    {
        return glm::perspective(glm::radians(Fov), static_cast<float>(Width) / static_cast<float>(Height), NearPlane, FarPlane);
    }

    glm::mat4 GetViewMatrix(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) const
    {
        return glm::lookAt(position, position + front, up);
    }

    float Fov;
    int Width;
    int Height;
    float NearPlane;
    float FarPlane;
};