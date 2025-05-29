#include "Scene/SceneLoader.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Mesh.h"
#include "ECS/Components/Material.h"
#include "ECS/Behaviors/CameraController.h"
#include "ECS/Components/Script.h"
#include "Assets/ShaderRegistry.h"
#include "ECS/Components/PointLight.h"
#include "Renderer/Material/Shaders/Binders/NormalBinder.h"


void CreateLights(Scene* scene)
{
    Entity light1(scene);
    light1.AddComponent<Transform>();
    light1.AddComponent<PointLight>(glm::vec3(23.47, 21.31, 20.79));

    Entity light2(scene);
    light2.AddComponent<Transform>();
    light2.AddComponent<PointLight>(glm::vec3(23.47, 21.31, 20.79));

    Entity light3(scene);
    light3.AddComponent<Transform>();
    light3.AddComponent<PointLight>(glm::vec3(23.47, 21.31, 20.79));

    Entity light4(scene);
    light4.AddComponent<Transform>();
    light4.AddComponent<PointLight>(glm::vec3(23.47, 21.31, 20.79));
}

void SceneLoader::Load(Scene* scene, const std::filesystem::path& filePath, const Project& project, const Window* window, const AssetManager& assetManager)
{
    scene->m_Registry.clear();

    const ShaderProgram* program = assetManager.GetRegistry<ShaderRegistry>()->LoadShaderProgram
    (
        project.GetShaderDirectory() / "Normal",
        "Normal"
    );

    std::vector<ShaderPipeline> pipelines;
    pipelines.emplace_back(program, std::make_shared<NormalBinder>());

    LoadObj(scene, pipelines, project.GetMeshDirectory() / "lion/lion_head_4k.obj", assetManager);

    Entity cameraEntity = Entity(scene);
    scene->m_CameraHandle = cameraEntity.GetID();
    cameraEntity.AddComponent<Camera>(glm::radians(45.f), 0.01f, 1000.0f);
    cameraEntity.AddComponent<Transform>(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Script& script = cameraEntity.AddComponent<Script>();
    script.AddBehavior<CameraController>(cameraEntity, 5.f, 0.01f, window);

    CreateLights(scene);
}

void SceneLoader::LoadObj(Scene* scene, const std::vector<ShaderPipeline>& pipelines, const std::filesystem::path& filePath, const AssetManager& assetManager)
{
    const MeshHandle* mesh = assetManager.LoadObjFile(filePath);

    Entity entity(scene);
    entity.AddComponent<Transform>();
    Material& material = entity.AddComponent<Material>();
    material.pipelines = pipelines;
    entity.AddComponent<Mesh>(mesh);
}
