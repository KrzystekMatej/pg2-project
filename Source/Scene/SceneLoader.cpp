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
#include "Renderer/Renderer.h"
#include "Config.h"

void SceneLoader::Load(Scene* scene, const std::filesystem::path& filePath, const Project& project, const Window* window, const AssetManager& assetManager)
{
    scene->m_Registry.clear();

    CreateTestSpheres(scene, project, window, assetManager);
}

std::vector<ShaderPipeline> CreateDirectPBRPipelines(const MeshHandle* mesh, const Project& project, const AssetManager& assetManager)
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

std::vector<ShaderPipeline> CreateDiffusePBRPipelines(const MeshHandle* mesh, const Project& project, const AssetManager& assetManager)
{
    const ShaderProgram* program = assetManager.GetRegistry<ShaderRegistry>()->LoadShaderProgram
    (
        project.GetShaderDirectory() / "PBR" / "DiffusePBR",
        "DiffusePBR"
    );

    assert(program && "Program is null!");

    const Texture* irradianceMap = Renderer::IrradianceMapPass
    (
        project.GetAssetDirectory() / "Environments" / "NewportLoft",
        assetManager.GetRegistry<TextureRegistry>(),
        assetManager.GetRegistry<ShaderRegistry>(),
        project
    );

    std::vector<ShaderPipeline> pipelines;
    pipelines.emplace_back(program, std::make_shared<DiffusePBRBinder>(irradianceMap));

    assetManager.GetRegistry<ShaderRegistry>()->LoadShaderProgram
    (
        project.GetShaderDirectory() / "PBR" / "Background" / "BackgroundDiffuse",
        "background"
    );

    return pipelines;
}

void SceneLoader::CreateTestSpheres(Scene* scene, const Project& project, const Window* window, const AssetManager& assetManager)
{
    const MeshHandle* mesh = assetManager.LoadObjFile(project.GetMeshDirectory() / "sphere/mesh.obj");

    std::vector<ShaderPipeline> pipelines = CreateDiffusePBRPipelines(mesh, project, assetManager);


    glm::vec3 lightPositions[] = { glm::vec3(0.0f, 0.0f, 10.0f) };
    glm::vec3 lightColors[] = { glm::vec3(150.0f, 150.0f, 150.0f) };

    int rows = 7;
    int columns = 7;
    float spacing = 2.5;

    glm::mat4 model = glm::mat4(1.0f);
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns; ++col)
        {
            Entity sphere(scene);

            sphere.AddComponent<Transform>
            (
                glm::vec3((float)(col - (columns / 2)) * spacing, (float)(row - (rows / 2)) * spacing, 0),
                glm::vec3(0, 0, 0),
                glm::vec3(1, 1, 1)
            );
            sphere.AddComponent<Mesh>(mesh);
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
        lightSphere.AddComponent<Mesh>(mesh);
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