#include "Renderer/Renderer.h"
#include "Renderer/Buffers/FrameBuffer.h"
#include "Renderer/Buffers/RenderBuffer.h"
#include "Assets/TextureFactory.h"
#include <glm/gtc/type_ptr.hpp>

int PrepareCube(VertexArray& vertexArray)
{
    float vertices[] =
    {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
         // bottom face
         -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
          1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
          1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
          1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
         -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         // top face
         -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
          1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
          1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
          1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };

    VertexBuffer vertexBuffer(vertices, sizeof(vertices));

    vertexBuffer.Bind();
    vertexArray.Bind();

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);

    vertexArray.AddBuffer(std::move(vertexBuffer), layout);
    vertexArray.Unbind();

    return sizeof(vertices) / (sizeof(float) * 8);
}

void Renderer::DrawPass(entt::registry& registry, const DrawContext& drawCtx)
{
    glDepthFunc(GL_LEQUAL);
    auto group = registry.group<Transform, Material, Mesh>();

    for (auto&& [entity, transform, material, geometry] : group.each())
    {
        for (const ShaderPipeline& pipeline : material.pipelines)
        {
            pipeline.Activate(drawCtx, transform, *geometry.mesh);
        }
        glDrawElements(GL_TRIANGLES, geometry.mesh->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}

void Renderer::DiffuseBackgroundPass(const ShaderProgram& backgroundShader, const Texture& backgroundTexture, const glm::mat4 projection, const glm::mat4& view)
{
    glDepthFunc(GL_LEQUAL);
    VertexArray cubeArray;
    int vertexCount = PrepareCube(cubeArray);
    backgroundShader.Use();
    backgroundShader.SetInt32("environmentMap", 0);
    Texture::Activate(0);
    backgroundTexture.Bind();
    backgroundShader.SetMatrix4x4("projection", glm::value_ptr(projection));
    backgroundShader.SetMatrix4x4("view", glm::value_ptr(view));
    cubeArray.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

const Texture* Renderer::IrradianceMapPass(std::filesystem::path directoryPath, TextureRegistry* textureRegistry, ShaderRegistry* shaderRegistry, const Project& project)
{
    glDepthFunc(GL_LEQUAL);
    FrameBuffer frameBuffer;
    RenderBuffer renderBuffer;

    constexpr uint32_t cubeMapSize = 512;
    
    frameBuffer.Bind();
    renderBuffer.Bind();
    renderBuffer.Storage(GL_DEPTH_COMPONENT24, cubeMapSize, cubeMapSize);
    frameBuffer.AttachRenderBuffer(GL_DEPTH_ATTACHMENT, renderBuffer);

    std::unique_ptr<GLImage> envImage = GLImage::LoadImage(directoryPath / "env.hdr", ColorSpace::Linear);
    std::unique_ptr<Texture> equirectangularMap = TextureFactory::CreateFromImage(*envImage, false);
    
    const Texture* cubeMap = textureRegistry->AddAsset
    (
        "background",
        std::move(TextureFactory::CreateEmptyCubeMap(cubeMapSize, GL_RGB32F, GL_RGB, GL_FLOAT, false))
    );
    cubeMap->Bind();

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    VertexArray cubeArray;
    int vertexCount = PrepareCube(cubeArray);

    const ShaderProgram* convertProgram = shaderRegistry->LoadShaderProgram
    (
        project.GetShaderDirectory() / "IBL" / "EquirectangularToCubeMap",
        "EquirectangularToCubeMap"
    );

    convertProgram->Use();
    convertProgram->SetInt32("equirectangularMap", 0);
    convertProgram->SetMatrix4x4("projection", glm::value_ptr(captureProjection));
    Texture::Activate(0);
    equirectangularMap->Bind();

    glViewport(0, 0, cubeMapSize, cubeMapSize);
    frameBuffer.Bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        cubeArray.Bind();
        convertProgram->SetMatrix4x4("view", glm::value_ptr(captureViews[i]));
        frameBuffer.AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMap->GetId(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
    cubeArray.Unbind();
    frameBuffer.Unbind();

    constexpr uint32_t irradianceMapSize = 32;

    
    const Texture* irradianceMap = textureRegistry->AddAsset
    (
        directoryPath.filename().string() + "_IrradianceMap",
        std::move(TextureFactory::CreateEmptyCubeMap(irradianceMapSize, GL_RGB16F, GL_RGB, GL_FLOAT, false))
    );

    frameBuffer.Bind();
    renderBuffer.Bind();
    renderBuffer.Storage(GL_DEPTH_COMPONENT24, irradianceMapSize, irradianceMapSize);

    const ShaderProgram* convolutionProgram = shaderRegistry->LoadShaderProgram(project.GetShaderDirectory() / "IBL" / "IrradianceConvolution", "IrradianceConvolution");
    convolutionProgram->Use();
    convolutionProgram->SetInt32("environmentMap", 0);
    convolutionProgram->SetMatrix4x4("projection", glm::value_ptr(captureProjection));

    glViewport(0, 0, irradianceMapSize, irradianceMapSize);
    frameBuffer.Bind();
    cubeArray.Bind();
    Texture::Activate(0);
    cubeMap->Bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        convolutionProgram->SetMatrix4x4("view", glm::value_ptr(captureViews[i]));
        frameBuffer.AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap->GetId(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
    cubeArray.Unbind();
    frameBuffer.Unbind();

    return irradianceMap;
}