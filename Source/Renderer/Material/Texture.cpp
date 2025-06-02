#include <glad/glad.h>
#include "Renderer/Material/Texture.h"

Texture::~Texture()
{
    if (m_Handle != 0)
    {
        glMakeTextureHandleNonResidentARB(m_Handle);
        m_Handle = 0;
    }

    if (m_Id)
    {
        glDeleteTextures(1, &m_Id);
        m_Id = 0;
    }
}

void Texture::Bind() const
{
	glBindTexture(m_Target, m_Id);
}
void Texture::Unbind() const
{
	glBindTexture(m_Target, 0);
}

void Texture::Activate(uint32_t index)
{
    glActiveTexture(GL_TEXTURE0 + index);
}
