#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include "AssetRegistry.h"
#include "GLImage.h"
#include "Renderer/Material/Texture.h"
#include "Renderer/Material/Shaders/ShaderProgram.h"
#include "Config.h"

class TextureRegistry : public AssetRegistry<Texture>
{
public:
    using BaseRegistryType = AssetRegistry;
    using AssetRegistry::AssetRegistry;

    const Texture* RegisterEmptyTexture(uint32_t width, uint32_t height);
    const Texture* RegisterPixelTexture(glm::vec3 color);
    const Texture* RegisterImageTexture(const GLImage& image, const std::string& textureName);
    const Texture* RegisterMipChain(const std::vector<std::unique_ptr<GLImage>>& mips, const std::string& textureName);
};
