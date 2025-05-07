#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include "AssetRegistry.h"
#include "Renderer/Texture.h"

class TextureRegistry : public AssetRegistry<Texture>
{
public:
    using BaseRegistryType = AssetRegistry;
    using AssetRegistry::AssetRegistry;

    const Texture* CreateSolidTexture(const glm::vec3 color);
    const Texture* LoadTexture(const std::filesystem::path& filePath, const std::string& textureName);
    const Texture* LoadMipChain(const std::filesystem::path& directoryPath, const std::string& textureName, TextureFormat textureFormat);
};
