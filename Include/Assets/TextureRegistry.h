#pragma once
#include <filesystem>
#include "AssetRegistry.h"
#include "Renderer/Texture.h"

class TextureRegistry : public AssetRegistry<Texture>
{
public:
    using BaseRegistryType = AssetRegistry;

    const Texture* LoadTexture(const std::filesystem::path& directoryPath, const std::string& fileName, TextureType type = TextureType::Generic);
    const Texture* LoadMipChain(const std::filesystem::path& directoryPath, const std::string& textureName, TextureFormat format, TextureType type);
};
