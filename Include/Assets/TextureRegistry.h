#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include "AssetRegistry.h"
#include "GLImage.h"
#include "Renderer/Texture.h"

class TextureRegistry : public AssetRegistry<Texture>
{
public:
    using BaseRegistryType = AssetRegistry;
    using AssetRegistry::AssetRegistry;

    const Texture* CreateEmptyTexture(uint32_t width, uint32_t height);
    const Texture* CreatePixelTexture(glm::vec3 color);
    const Texture* CreateTexture(const GLImage& image, const std::string& textureName);
    const Texture* CreateMipChain(const std::vector<std::unique_ptr<GLImage>>& mips, const std::string& textureName);
};
