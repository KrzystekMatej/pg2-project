#pragma once
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"

class RenderSystem
{
public:

    void Draw(Scene& scene) const;
private:
    Renderer m_Renderer = Renderer(GL_TRIANGLES);//musíš upravit - prozatímní
};
