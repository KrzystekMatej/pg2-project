#pragma once
#include "ECS/Components/PointLight.h"
#include "ECS/Components/Transform.h"
#include <entt/entt.hpp>
#include <utility>

struct RenderContext 
{
    glm::mat4 Projection;
    glm::mat4 View;
    glm::vec3 CameraPosition;

    entt::view<entt::get_t<Transform, PointLight>> LightView;
};