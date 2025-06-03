#include "Renderer/Renderer.h"
#include "Renderer/Buffers/FrameBuffer.h"
#include "Renderer/Buffers/RenderBuffer.h"
#include "Assets/TextureFactory.h"
#include <glm/gtc/type_ptr.hpp>
#include "ECS/Components/PointLight.h"

std::unique_ptr<VertexArray> cubeArray = nullptr;
uint32_t cubeVertexCount = 0;

void PrepareCube()
{
    cubeArray = std::make_unique<VertexArray>();
    
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

    VertexBuffer cubeBuffer(vertices, sizeof(vertices));

    cubeBuffer.Bind();
    cubeArray->Bind();

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);

    cubeArray->AddBuffer(std::move(cubeBuffer), layout);
    cubeArray->Unbind();

    cubeVertexCount = sizeof(vertices) / (sizeof(float) * 8);
}

void Renderer::ShadowPass(const ShaderProgram& shadowShader, entt::registry& registry, const DrawContext& drawCtx)
{
    if (Renderer::s_ShadowFBO.GetId() == 0)
    {
        Renderer::s_ShadowFBO = FrameBuffer(true);
    }

    constexpr uint32_t SHADOW_SIZE = 1024;
    glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
    glCullFace(GL_FRONT);
    glDepthFunc(GL_LESS);
    uint32_t lightIdx = 0;
    shadowShader.Use();

    for (auto [lightEntity, lightTransform, light] : drawCtx.LightView.each())
    {
        const Texture* depthCube = PointLight::s_DepthMaps[lightIdx].get();
        s_ShadowFBO.Bind();
        s_ShadowFBO.AttachTexture(GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, depthCube->GetId());
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glClear(GL_DEPTH_BUFFER_BIT);

        const glm::vec3 position = lightTransform.Position;
        const float nearP = 1.0f;
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, nearP, light.FarPlane);

        glm::mat4 mats[6];
        mats[0] = projection * glm::lookAt(position, position + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0));
        mats[1] = projection * glm::lookAt(position, position + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
        mats[2] = projection * glm::lookAt(position, position + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
        mats[3] = projection * glm::lookAt(position, position + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
        mats[4] = projection * glm::lookAt(position, position + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0));
        mats[5] = projection * glm::lookAt(position, position + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));

        shadowShader.SetMatrix4x4Array("shadowMatrices", 6, mats);
        shadowShader.SetVec3("lightPosition", glm::value_ptr(position));
        shadowShader.SetFloat("farPlane", light.FarPlane);

        auto group = registry.group<Transform, Material, Mesh>();

        for (auto&& [entity, transform, material, geometry] : group.each())
        {
            glm::mat4 model = transform.GetLocalToWorldMatrix();
            shadowShader.SetMatrix4x4("model", model);

            geometry.mesh->GetVertexArray().Bind();
            glDrawElements(GL_TRIANGLES, geometry.mesh->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
        }

        lightIdx++;
    }

    s_ShadowFBO.Unbind();
    glCullFace(GL_BACK);
}

void Renderer::DrawPass(entt::registry& registry, const DrawContext& drawCtx)
{
    glDepthFunc(GL_LESS);
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
    if (!cubeArray) PrepareCube();
    backgroundShader.Use();
    backgroundShader.SetInt32("environmentMap", 0);
    Texture::Activate(0);
    backgroundTexture.Bind();
    backgroundShader.SetMatrix4x4("projection", projection);
    backgroundShader.SetMatrix4x4("view", view);
    cubeArray->Bind();
    glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);
}

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

const Texture* Renderer::CubeMapPass
(
    std::filesystem::path envPath,
    TextureRegistry* textureRegistry,
    ShaderRegistry* shaderRegistry,
    const Project& project,
    uint32_t size
)
{
    glDepthFunc(GL_LEQUAL);
    FrameBuffer frameBuffer;
    RenderBuffer renderBuffer;

    frameBuffer.Bind();
    renderBuffer.Bind();
    renderBuffer.Storage(GL_DEPTH_COMPONENT24, size, size);
    frameBuffer.AttachRenderBuffer(GL_DEPTH_ATTACHMENT, renderBuffer);

    std::unique_ptr<GLImage> envImage = GLImage::LoadImage(envPath, ColorSpace::Linear, false);
    std::unique_ptr<Texture> equirectangularMap = TextureFactory::CreateFromImage(*envImage, true);

    const Texture* cubeMap = textureRegistry->AddAsset
    (
        "Background",
        std::move(TextureFactory::CreateEmptyCubeMap(size, GL_RGB32F, GL_RGB, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, false))
    );
    cubeMap->Bind();

    const ShaderProgram* convertShader = shaderRegistry->LoadShaderProgram
    (
        project.GetShaderDirectory() / "IBL" / "EquirectangularToCubeMap",
        "EquirectangularToCubeMap"
    );

    if (!cubeArray) PrepareCube();

    convertShader->Use();
    convertShader->SetInt32("equirectangularMap", 0);
    convertShader->SetMatrix4x4("projection", captureProjection);
    Texture::Activate(0);
    equirectangularMap->Bind();

    glViewport(0, 0, size, size);
    frameBuffer.Bind();
    for (uint32_t i = 0; i < 6; i++)
    {
        cubeArray->Bind();
        convertShader->SetMatrix4x4("view", captureViews[i]);
        frameBuffer.AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMap->GetId(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);
    }
    cubeArray->Unbind();
    frameBuffer.Unbind();

    cubeMap->Bind();
    cubeMap->GenerateMipmaps();

    return cubeMap;
}

const Texture* Renderer::IrradianceMapPass
(
    std::filesystem::path directoryPath,
    TextureRegistry* textureRegistry,
    ShaderRegistry* shaderRegistry,
    const Project& project,
    const Texture* cubeMap,
    uint32_t size,
    float sampleDelta
)
{
    glDepthFunc(GL_LEQUAL);
    FrameBuffer frameBuffer;
    RenderBuffer renderBuffer;

    
    const Texture* irradianceMap = textureRegistry->AddAsset
    (
        directoryPath.filename().string() + "_IrradianceMap",
        std::move(TextureFactory::CreateEmptyCubeMap(size, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, GL_LINEAR, false))
    );

    frameBuffer.Bind();
    renderBuffer.Bind();
    renderBuffer.Storage(GL_DEPTH_COMPONENT24, size, size);

    const ShaderProgram* convolutionShader = shaderRegistry->LoadShaderProgram
    (
        project.GetShaderDirectory() / "IBL" / "IrradianceConvolution",
        "IrradianceConvolution"
    );
    convolutionShader->Use();
    convolutionShader->SetFloat("sampleDelta", sampleDelta);
    convolutionShader->SetInt32("environmentMap", 0);
    convolutionShader->SetMatrix4x4("projection", captureProjection);

    glViewport(0, 0, size, size);
    frameBuffer.Bind();
    cubeArray->Bind();
    Texture::Activate(0);
    cubeMap->Bind();
    for (uint32_t i = 0; i < 6; i++)
    {
        convolutionShader->SetMatrix4x4("view", captureViews[i]);
        frameBuffer.AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap->GetId(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);
    }
    cubeArray->Unbind();
    frameBuffer.Unbind();

    return irradianceMap;
}

const Texture* Renderer::PrefilterMapPass
(
    std::filesystem::path directoryPath,
    TextureRegistry* textureRegistry,
    ShaderRegistry* shaderRegistry,
    const Project& project,
    const Texture* cubeMap,
    uint32_t size,
    uint32_t maxMipLevels,
    uint32_t sampleCount,
    float resolution
)
{
    uint32_t maxPossibleLevels = static_cast<uint32_t>(std::floor(std::log2(size))) + 1;
    if (maxMipLevels > maxPossibleLevels)
    {
        spdlog::warn
        (
            "Requested maxMipLevels ({}) exceeds the maximum possible for size {} ({} levels). Clamping to {}.",
            maxMipLevels,
            size,
            maxPossibleLevels,
            maxPossibleLevels
        );
        maxMipLevels = maxPossibleLevels;
    }

    glDepthFunc(GL_LEQUAL);
    FrameBuffer frameBuffer;
    RenderBuffer renderBuffer;

    const Texture* prefilterMap = textureRegistry->AddAsset
    (
        directoryPath.filename().string() + "_PrefilterMap",
        std::move(TextureFactory::CreateEmptyCubeMap(size, GL_RGB32F, GL_RGB, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, false))
    );

    prefilterMap->Bind();
    prefilterMap->GenerateMipmaps();

    const ShaderProgram* prefilterShader = shaderRegistry->LoadShaderProgram
    (
        project.GetShaderDirectory() / "IBL" / "Prefilter",
        "Prefilter"
    );

    prefilterShader->Use();
    prefilterShader->SetUInt32("sampleCount", sampleCount);
    prefilterShader->SetFloat("resolution", resolution);
    prefilterShader->SetInt32("environmentMap", 0);
    prefilterShader->SetMatrix4x4("projection", captureProjection);
    Texture::Activate(0);
    cubeMap->Bind();
    frameBuffer.Bind();
    cubeArray->Bind();
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        unsigned int mipWidth = static_cast<unsigned int>(size * std::pow(0.5, mip));
        unsigned int mipHeight = static_cast<unsigned int>(size * std::pow(0.5, mip));
        renderBuffer.Bind();
        renderBuffer.Storage(GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader->SetFloat("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilterShader->SetMatrix4x4("view", captureViews[i]);
            frameBuffer.AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap->GetId(), mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);
        }
    }
    cubeArray->Unbind();
    frameBuffer.Unbind();
    return prefilterMap;
}

std::unique_ptr<VertexArray> quadArray = nullptr;
uint32_t quadVertexCount = 0;

void PrepareQuad()
{
    quadArray = std::make_unique<VertexArray>();

    float quadVertices[] = 
    {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    VertexBuffer quadBuffer(quadVertices, sizeof(quadVertices));

    quadBuffer.Bind();
    quadArray->Bind();

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);

    quadArray->AddBuffer(std::move(quadBuffer), layout);
    quadArray->Unbind();

    quadVertexCount = sizeof(quadVertices) / (sizeof(float) * 5);
}


const Texture* Renderer::BrdfTablePass
(
    std::filesystem::path directoryPath,
    TextureRegistry* textureRegistry,
    ShaderRegistry* shaderRegistry,
    const Project& project,
    uint32_t size,
    uint32_t sampleCount
)
{
    glDepthFunc(GL_LEQUAL);
    FrameBuffer frameBuffer;
    RenderBuffer renderBuffer;

    const Texture* brdfTable = textureRegistry->AddAsset
    (
        directoryPath.filename().string() + "_BrdfTable",
        std::move(TextureFactory::CreateEmpty2DLinear(size, size, GL_RG32F, GL_RG, GL_FLOAT, false))
    );

    const ShaderProgram* brdfShader = shaderRegistry->LoadShaderProgram
    (
        project.GetShaderDirectory() / "IBL" / "BRDF",
        "BRDF"
    );

    frameBuffer.Bind();
    renderBuffer.Bind();
    renderBuffer.Storage(GL_DEPTH_COMPONENT24, size, size);
    frameBuffer.AttachTexture(GL_COLOR_ATTACHMENT0, brdfTable->GetTarget(), brdfTable->GetId(), 0);

    if (!quadArray) PrepareQuad();

    glViewport(0, 0, size, size);
    brdfShader->Use();
    brdfShader->SetUInt32("sampleCount", sampleCount);
    quadArray->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, quadVertexCount);
    return brdfTable;
}
