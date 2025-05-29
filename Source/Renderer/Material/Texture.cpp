#include <glad/gl.h>
#include "Renderer/Material/Texture.h"

Texture::~Texture()
{
	if (m_Id)
	{
		glDeleteTextures(1, &m_Id);
		m_Id = 0;
	}
}