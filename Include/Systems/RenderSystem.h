#pragma once
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"

class RenderSystem
{
public:

    void Update(const Scene& scene);
private:
    Renderer m_Renderer = Renderer(GL_TRIANGLES);//mus� upravit - prozat�mn�
};
