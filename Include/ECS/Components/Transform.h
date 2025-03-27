#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
    glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
    glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

    Transform() = default;
    Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
        : Position(position), Rotation(rotation), Scale(scale) {}

    glm::mat4 GetMatrix() const
    {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, Position);
        matrix = glm::rotate(matrix, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
        matrix = glm::rotate(matrix, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
        matrix = glm::rotate(matrix, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
        matrix = glm::scale(matrix, Scale);
        return matrix;
    }


    glm::vec3 GetFrontVector() const
    {
        return glm::normalize(glm::vec3(
            cos(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x)),
            sin(glm::radians(Rotation.x)),
            sin(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x))
        ));
    }

    glm::vec3 GetUpVector() const
    {
        glm::vec4 up = GetMatrix() * glm::vec4(0, 1, 0, 0);
        return glm::normalize(glm::vec3(up));
    }
};