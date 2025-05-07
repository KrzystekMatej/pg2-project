#include "Scene/SceneLoader.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Mesh.h"
#include "ECS/Components/Material.h"
#include "ECS/Behaviors/CameraController.h"
#include "ECS/Components/Script.h"
#include "Assets/ShaderRegistry.h"
#include "Renderer/ShaderStorageBuffer.h"

void SceneLoader::Load(Scene* scene, const std::filesystem::path& filePath, const Project& project, const Window* window, const AssetManager& assetManager)
{
    scene->m_Registry.clear();

    const ShaderProgram* program = assetManager.GetRegistry<ShaderRegistry>()->LoadShaderProgram
    (
        project.GetShaderDirectory() / "Normal",
        "Normal"
    );

    LoadObj(scene, program, project.GetMeshDirectory() / "lion/lion_head_4k.obj", assetManager);

    Entity cameraEntity = Entity(scene);
    scene->m_CameraHandle = cameraEntity.GetID();
    cameraEntity.AddComponent<Camera>(glm::radians(45.f), 0.01f, 1000.0f);
    cameraEntity.AddComponent<Transform>(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Script& script = cameraEntity.AddComponent<Script>();
    script.AddBehavior<CameraController>(cameraEntity, 5.f, 0.01f, window);
}

void SceneLoader::LoadObj(Scene* scene, const ShaderProgram* program, const std::filesystem::path& filePath, const AssetManager& assetManager)
{
    const MeshHandle* mesh = assetManager.LoadObjFile(filePath);

    Entity entity(scene);
    entity.AddComponent<Transform>();
    entity.AddComponent<Material>(program);
    entity.AddComponent<Mesh>(mesh);
}
