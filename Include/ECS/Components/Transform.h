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

    glm::mat4 GetTransformMatrix() const
    {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, Position);
        matrix = glm::rotate(matrix, Rotation.x, glm::vec3(1, 0, 0));
        matrix = glm::rotate(matrix, Rotation.y, glm::vec3(0, 1, 0));
        matrix = glm::rotate(matrix, Rotation.z, glm::vec3(0, 0, 1));
        matrix = glm::scale(matrix, Scale);
        return matrix;
    }
};