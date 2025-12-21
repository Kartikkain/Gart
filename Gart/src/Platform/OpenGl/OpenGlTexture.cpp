#include "bsspch.h"
#include "OpenGlTexture.h"
#include "stb_image.h"
#include "Core/Log.h"

namespace Gart
{
	namespace Utils
	{
		static GLenum GartFormatToDataFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8: return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;
			}
			BSS_CORE_ASSERT(false, "No Such Format Found.");
			return 0;
		}

		static GLenum GartFormatToInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case Gart::ImageFormat::RGB8: return GL_RGB8;
			case Gart::ImageFormat::RGBA8: return GL_RGBA8;
			}

			BSS_CORE_ASSERT(false, "No Such Format Found.");
			return 0;
		}
	}

	OpenGlTexture::OpenGlTexture(const TextureSpecifications& spec)
		:m_TextureSpec(spec), m_Width(spec.m_width), m_Height(spec.m_height)
	{
		GART_PROFILE_FUNCTION();

		m_InternalFormat = Utils::GartFormatToInternalFormat(m_TextureSpec.m_imageFormat);
		m_DataFormat = Utils::GartFormatToDataFormat(m_TextureSpec.m_imageFormat);


		glCreateTextures(GL_TEXTURE_2D, 1, &m_RenderID);
		glTextureStorage2D(m_RenderID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RenderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RenderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

	OpenGlTexture::OpenGlTexture(const std::string& path)
		:m_Path(path)
	{
		GART_PROFILE_FUNCTION();

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

		m_InternalFormat = InternalFormat;
		m_DataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RenderID);
		glTextureStorage2D(m_RenderID, 1, InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RenderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RenderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RenderID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

	}
	OpenGlTexture::~OpenGlTexture()
	{
		GART_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RenderID);
	}

	void OpenGlTexture::SetData(void* data, uint32_t size)
	{
		GART_PROFILE_FUNCTION();

		glTextureSubImage2D(m_RenderID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGlTexture::Bind(uint32_t slot) const
	{
		GART_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RenderID);
	}

}