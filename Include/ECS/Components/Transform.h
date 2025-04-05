#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform
{
    static constexpr glm::vec3 WorldUp{ 0.0f, 1.0f, 0.0f };

    glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
    glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

    Transform() = default;
    Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
        : Position(position), Rotation(rotation), Scale(scale) {}


	glm::mat4 GetLocalToWorldMatrix() const
    {
        glm::vec3 forward = GetForwardVector();
        glm::vec3 right = glm::normalize(glm::cross(forward, WorldUp));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        glm::vec3 look = -forward;

        return glm::mat4
        {
            right.x * Scale.x,  right.y * Scale.x,  right.z * Scale.x,  0.0f,
            up.x * Scale.y,     up.y * Scale.y,     up.z * Scale.y,     0.0f,
            look.x * Scale.z,   look.y * Scale.z,   look.z * Scale.z,   0.0f,
            Position.x,         Position.y,         Position.z,         1.0f
        };
    }

    glm::mat3 GetNormalMatrix() const
    {
        glm::vec3 forward = GetForwardVector();
        glm::vec3 right = glm::normalize(glm::cross(forward, WorldUp));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        glm::vec3 look = -forward;

        return glm::mat3
        {
            right.x * Scale.x,  right.y * Scale.x,  right.z * Scale.z,
            up.x * Scale.y,     up.y * Scale.y,     up.z * Scale.z,
            look.x * Scale.z,   look.y * Scale.z,   look.z * Scale.z,
        };
    }

    glm::mat4 GetWorldToLocalMatrix() const
    {
        glm::vec3 inverseScale = 1.0f / Scale;

        glm::vec3 forward = GetForwardVector();
        glm::vec3 right = glm::normalize(glm::cross(forward, WorldUp));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        glm::vec3 look = -forward;

        glm::vec3 t =
        {
            glm::dot(right, -Position),
        	glm::dot(up, -Position),
            glm::dot(look, -Position)
        };

        return glm::mat4
        {
            right.x * inverseScale.x,  right.y * inverseScale.x,  right.z * inverseScale.x,  0.0f,
            up.x * inverseScale.y,     up.y * inverseScale.y,     up.z * inverseScale.y,     0.0f,
            look.x * inverseScale.z,   look.y * inverseScale.z,   look.z * inverseScale.z,   0.0f,
            t.x,                       t.y,                       t.z,                       1.0f
        };

    }

    glm::mat4 GetViewMatrix() const
    {
        glm::vec3 forward = GetForwardVector();
        glm::vec3 right = normalize(glm::cross(forward, WorldUp));
        glm::vec3 up = normalize(glm::cross(right, forward));

        glm::vec3 look = -forward;

        glm::vec3 t =
        {
            glm::dot(right, -Position),
            glm::dot(up, -Position),
            glm::dot(look, -Position)
        };

        return glm::mat4
    	{
            right.x, up.x, look.x, 0.0f,
            right.y, up.y, look.y, 0.0f,
            right.z, up.z, look.z, 0.0f,
            t.x, t.y, t.z, 1.0f
        };
    }



    glm::vec3 GetForwardVector() const
    {
        float yaw = Rotation.y;
        float pitch = Rotation.x;

        return glm::normalize
    	(
            glm::vec3
            (
				sin(yaw) * cos(pitch),
				sin(pitch),
				-cos(yaw) * cos(pitch)
			)
        );
    }
};