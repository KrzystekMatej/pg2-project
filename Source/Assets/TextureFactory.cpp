#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include "Assets/TextureFactory.h"

std::unique_ptr<Texture> TextureFactory::CreateEmpty(uint32_t width, uint32_t height, bool makeResident)
{
    if (width == 0 || height == 0) return nullptr;

    uint32_t id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    glBindTexture(GL_TEXTURE_2D, 0);

    return CreateTextureWrapper(id, GL_TEXTURE_2D, width, height, makeResident);
}

std::unique_ptr<Texture> TextureFactory::CreatePixel(glm::vec3 color, bool makeResident)
{
    uint8_t pixel[4] =
    {
        static_cast<uint8_t>(glm::clamp(color.r, 0.0f, 1.0f) * 255.0f),
        static_cast<uint8_t>(glm::clamp(color.g, 0.0f, 1.0f) * 255.0f),
        static_cast<uint8_t>(glm::clamp(color.b, 0.0f, 1.0f) * 255.0f),
        255
    };

    uint32_t id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    glBindTexture(GL_TEXTURE_2D, 0);

    return CreateTextureWrapper(id, GL_TEXTURE_2D, 1, 1, makeResident);
}

std::unique_ptr<Texture> TextureFactory::CreateFromImage(const GLImage& image, bool makeResident)
{
    BYTE* pixels = FreeImage_GetBits(image.Bitmap);
    if (!pixels)
    {
        spdlog::error("CreateFromImage: Failed to get pixel data image");
        return nullptr;
    }

    uint32_t id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        image.InternalFormat,
        static_cast<int>(image.Width),
        static_cast<int>(image.Height),
        0,
        image.ExternalFormat,
        image.DataType,
        pixels
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return CreateTextureWrapper(id, GL_TEXTURE_2D, image.Width, image.Height, makeResident);
}

std::unique_ptr<Texture> TextureFactory::CreateFromMipChain(const std::vector<std::unique_ptr<GLImage>>& mips, bool makeResident)
{
    if (mips.empty())
    {
        spdlog::warn("CreateFromMipChain: Missing mip images");
        return {};
    }

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(mips.size() - 1));

    uint32_t width = 0;
    uint32_t height = 0;

    for (size_t i = 0; i < mips.size(); i++)
    {
        const auto& image = mips[i];
        BYTE* pixels = FreeImage_GetBits(image->Bitmap);
        if (!pixels)
        {
            spdlog::error("CreateFromMipChain: Failed to get pixel data from mip level {}");
            return nullptr;
        }

        glTexImage2D
        (
            GL_TEXTURE_2D,
            static_cast<int>(i),
            image->InternalFormat,
            static_cast<int>(image->Width),
            static_cast<int>(image->Height),
            0,
            image->ExternalFormat,
            image->DataType,
            pixels
        );
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return CreateTextureWrapper(id, GL_TEXTURE_2D, width, height, makeResident);
}

std::unique_ptr<Texture> TextureFactory::CreateEmptyCubeMap(uint32_t size, GLint internalFormat, GLenum externalFormat, GLenum dataType, bool makeResident)
{
    if (size == 0) return nullptr;

    uint32_t id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            internalFormat,
            size,
            size,
            0,
            externalFormat,
            dataType,
            nullptr
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return CreateTextureWrapper(id, GL_TEXTURE_CUBE_MAP, size, size, makeResident);
}

std::unique_ptr<Texture> TextureFactory::CreateTextureWrapper(uint32_t id, uint32_t target, uint32_t width, uint32_t height, bool makeResident)
{
    if (!makeResident) return std::make_unique<Texture>(id, target, 0, width, height);
    uint64_t handle = glGetTextureHandleARB(id);

    if (!glIsTexture(id))
    {
        spdlog::error("glIsTexture failed for ID {}", id);
        return nullptr;
    }

    glMakeTextureHandleResidentARB(handle);

    if (!glIsTextureHandleResidentARB(handle))
    {
        spdlog::error("Handle {} is not resident!", handle);
        glDeleteTextures(1, &id);
        return nullptr;
    }

    if (!handle) return nullptr;
    return std::make_unique<Texture>(id, target, handle, width, height);
}
