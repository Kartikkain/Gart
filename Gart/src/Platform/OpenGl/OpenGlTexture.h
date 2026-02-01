#pragma once

#include "Renderer/Texture.h"
#include <glad/glad.h>

namespace Gart
{
	class OpenGlTexture: public Texture2D
	{
	public:
		OpenGlTexture(const TextureSpecifications& spec, Buffer data=Buffer());
		virtual ~OpenGlTexture();
		virtual const TextureSpecifications& GetTextureSpecs() const override { return m_TextureSpec; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRenderID() const override { return m_RenderID; }
		virtual void SetData(Buffer data) override;
		virtual const std::string GetPath() const override { return m_Path; }
		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator== (const Texture& other) const override
		{
			return m_RenderID == ((OpenGlTexture&)other).m_RenderID;
		}

	private:
		TextureSpecifications m_TextureSpec;
		std::string m_Path;
		uint32_t m_Width,m_Height;
		uint32_t m_RenderID;
		GLenum m_InternalFormat, m_DataFormat;

	};
}
