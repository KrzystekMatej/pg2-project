#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Renderer/Material/Texture.h"
#include "GLImage.h"

class TextureFactory
{
public:
    static std::unique_ptr<Texture> CreateEmpty(uint32_t width, uint32_t height, bool makeResident = true);
    static std::unique_ptr<Texture> CreatePixel(glm::vec3 color, bool makeResident = true);
    static std::unique_ptr<Texture> CreateFromImage(const GLImage& image, bool makeResident = true);
    static std::unique_ptr<Texture> CreateFromMipChain(const std::vector<std::unique_ptr<GLImage>>& mips, bool makeResident = true);
    static std::unique_ptr<Texture> CreateEmptyCubeMap(uint32_t size, GLint internalFormat, GLenum externalFormat, GLenum dataType, bool makeResident = true);
private:
    static std::unique_ptr<Texture> CreateTextureWrapper(uint32_t id, uint32_t target, uint32_t width, uint32_t height, bool makeResident);
};