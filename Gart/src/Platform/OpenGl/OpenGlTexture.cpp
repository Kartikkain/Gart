#include "bsspch.h"
#include "OpenGlTexture.h"
#include <glad/glad.h>
#include "stb_image.h"

namespace Gart
{
	OpenGlTexture::OpenGlTexture(const std::string& path)
		:m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);

		m_Width = width;
		m_Height = height;
		GLenum InternalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			InternalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			InternalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}


		glCreateTextures(GL_TEXTURE_2D, 1, &m_RenderID);
		glTextureStorage2D(m_RenderID, 1, InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RenderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RenderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RenderID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

	}
	OpenGlTexture::~OpenGlTexture()
	{
		glDeleteTextures(1, &m_RenderID);
	}

	void OpenGlTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RenderID);
	}

}