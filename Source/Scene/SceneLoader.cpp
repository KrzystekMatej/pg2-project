#include "Scene/SceneLoader.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Mesh.h"
#include "ECS/Components/Material.h"
#include "ECS/Behaviors/CameraController.h"
#include "ECS/Components/Script.h"
#include "Assets/ShaderRegistry.h"
#include "ECS/Components/PointLight.h"
#include "Renderer/Material/Shaders/Binders/NormalBinder.h"
#include "Renderer/Material/Shaders/Binders/DirectPBRBinder.h"
#include "Renderer/Material/Shaders/Binders/DiffusePBRBinder.h"
#include "Renderer/Material/Shaders/Binders/FullPBRBinder.h"
#include "Renderer/Renderer.h"
#include "Config.h"

void SceneLoader::Load(Scene* scene, const std::filesystem::path& filePath, const Project& project, const Window* window, const AssetManager& assetManager)
{
    scene->m_Registry.clear();

    CreateTestSpheres(scene, project, window, assetManager);
}

std::vector<ShaderPipeline> CreateDirectPBRPipelines(const Project& project, const AssetManager& assetManager)
{
    const ShaderProgram* program = assetManager.GetRegistry<ShaderRegistry>()->LoadShaderProgram
    (
        project.GetShaderDirectory() / "PBR" / "DirectPBR",
        "DirectPBR"
    );

    assert(program && "Program is null!");

    std::vector<ShaderPipeline> pipelines;
    pipelines.emplace_back(program, std::make_shared<DirectPBRBinder>());
    return pipelines;
}

std::vector<ShaderPipeline> CreateDiffusePBRPipelines(const Project& project, const AssetManager& assetManager, std::filesystem::path envPath)
{
    const ShaderProgram* program = assetManager.GetRegistry<ShaderRegistry>()->LoadShaderProgram
    (
        project.GetShaderDirectory() / "PBR" / "DiffusePBR",
        "DiffusePBR"
    );

    assert(program && "Program is null!");

    TextureRegistry* textureRegistry = assetManager.GetRegistry<TextureRegistry>();
    ShaderRegistry* shaderRegistry = assetManager.GetRegistry<ShaderRegistry>();
    std::filesystem::path directoryPath = envPath.parent_path();

    uint32_t environmentSize = 2048;
    uint32_t irradianceSize = 64;
    float sampleDelta = 0.01f;

    // uint32_t environmentSize = 512;
    // uint32_t irradianceSize = 32;
    // float sampleDelta = 0.025f;

    const Texture* cubeMap = Renderer::CubeMapPass
    (
        envPath,
        textureRegistry,
        shaderRegistry,
        project,
        environmentSize
    );

    const Texture* irradianceMap = Renderer::IrradianceMapPass
    (
        directoryPath,
        textureRegistry,
        shaderRegistry,
        project,
        cubeMap,
        irradianceSize,
        sampleDelta
    );

    std::vector<ShaderPipeline> pipelines;
    pipelines.emplace_back(program, std::make_shared<DiffusePBRBinder>(irradianceMap));

    assetManager.GetRegistry<ShaderRegistry>()->LoadShaderProgram
    (
        project.GetShaderDirectory() / "PBR" / "Background",
        "background"
    );

    return pipelines;
}

std::vector<ShaderPipeline> CreateFullPBRPipelines(const Project& project, const AssetManager& assetManager, std::filesystem::path envPath)
{
    const ShaderProgram* program = assetManager.GetRegistry<ShaderRegistry>()->LoadShaderProgram
    (
        project.GetShaderDirectory() / "PBR" / "FullPBR",
        "FullPBR"
    );

    assert(program && "Program is null!");

    TextureRegistry* textureRegistry = assetManager.GetRegistry<TextureRegistry>();
    ShaderRegistry* shaderRegistry = assetManager.GetRegistry<ShaderRegistry>();
    std::filesystem::path directoryPath = envPath.parent_path();


    uint32_t environmentSize = 2048;
    uint32_t irradianceSize = 64;
    uint32_t prefilterSize = 512;
    uint32_t brdfLutSize = 512;
    uint32_t prefilterSamples = 1024;
    float sampleDelta = 0.01f;
    uint32_t mipLevels = 5;
    uint32_t sampleCount = 1024;

    /*uint32_t environmentSize = 512;
    uint32_t irradianceSize = 32;
    uint32_t prefilterSize = 128;
    uint32_t brdfLutSize = 512;
    uint32_t prefilterSamples = 1024;
    float sampleDelta = 0.025f;
    uint32_t mipLevels = 5;
    uint32_t sampleCount = 1024;*/

    const Texture* cubeMap = Renderer::CubeMapPass
    (
        envPath,
        textureRegistry,
        shaderRegistry,
        project,
        environmentSize
    );

    const Texture* irradianceMap = Renderer::IrradianceMapPass
    (
        directoryPath,
        textureRegistry,
        shaderRegistry,
        project,
        cubeMap,
        irradianceSize,
        sampleDelta
    );

    const Texture* prefilterMap = Renderer::PrefilterMapPass
    (
        directoryPath,
        textureRegistry,
        shaderRegistry,
        project,
        cubeMap,
        prefilterSize,
        mipLevels,
        prefilterSamples,
        static_cast<float>(prefilterSize)
    );

    const Texture* brdfTable = Renderer::BrdfTablePass
    (
        directoryPath,
        textureRegistry,
        shaderRegistry,
        project,
        brdfLutSize,
        sampleCount
    );

    std::vector<ShaderPipeline> pipelines;
    pipelines.emplace_back(program, std::make_shared<FullPBRBinder>(irradianceMap, prefilterMap, brdfTable));

    assetManager.GetRegistry<ShaderRegistry>()->LoadShaderProgram
    (
        project.GetShaderDirectory() / "PBR" / "Background",
        "background"
    );

    return pipelines;
}

void SceneLoader::CreateTestSpheres(Scene* scene, const Project& project, const Window* window, const AssetManager& assetManager)
{
    std::vector<const MeshHandle*> meshes
    {
        assetManager.LoadObjFile(project.GetMeshDirectory() / "spheres/gold/mesh.obj"),
        assetManager.LoadObjFile(project.GetMeshDirectory() / "spheres/grass/mesh.obj"),
        assetManager.LoadObjFile(project.GetMeshDirectory() / "spheres/plastic/mesh.obj"),
        assetManager.LoadObjFile(project.GetMeshDirectory() / "spheres/rusted_iron/mesh.obj"),
        assetManager.LoadObjFile(project.GetMeshDirectory() / "spheres/wall/mesh.obj")
    };

    std::vector<ShaderPipeline> pipelines = CreateFullPBRPipelines(project, assetManager, project.GetAssetDirectory() / "Environments" / "KiaraDawn" / "env.exr");


    glm::vec3 lightPositions[] = 
    {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3(10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3(10.0f, -10.0f, 10.0f),
    };

    glm::vec3 lightColors[] =
    {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };

    int rows = meshes.size();
    int columns = meshes.size();
    float spacing = 2.5;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns; ++col)
        {
            Entity sphere(scene);

            sphere.AddComponent<Transform>
            (
                glm::vec3((float)(col - (columns / 2)) * spacing, (float)(row - (rows / 2)) * spacing, 0),
                glm::vec3(0, 0, 0),
                glm::vec3(2, 2, 2)
            );
            sphere.AddComponent<Mesh>(meshes[row]);
            Material& material = sphere.AddComponent<Material>();
            material.pipelines = pipelines;
        }
    }

    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
    {
        Entity lightSphere(scene);
        lightSphere.AddComponent<Transform>(
            lightPositions[i],
            glm::vec3(0, 0, 0),
            glm::vec3(0.5f)
        );
        lightSphere.AddComponent<Mesh>(meshes[0]);
        Material& material = lightSphere.AddComponent<Material>();
        material.pipelines = pipelines;
        lightSphere.AddComponent<PointLight>(lightColors[i]);
    }

    Entity cameraEntity = Entity(scene);
    scene->m_CameraHandle = cameraEntity.GetID();
    cameraEntity.AddComponent<Camera>(glm::radians(45.f), 0.01f, 1000.0f);
    cameraEntity.AddComponent<Transform>(glm::vec3(0, 0, 20), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Script& script = cameraEntity.AddComponent<Script>();
    script.AddBehavior<CameraController>(cameraEntity, 5.f, 0.01f, window);
}