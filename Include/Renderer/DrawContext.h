#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "ECS/Components/Transform.h"
#include "ECS/Components/PointLight.h"


struct DrawContext
{
    glm::mat4 Projection;
    glm::mat4 View;
    glm::vec3 CameraPosition;

    entt::view<entt::get_t<Transform, PointLight>> LightView;
};