#pragma once

#include "Renderer/Texture.h"

namespace Gart
{
	class OpenGlTexture: public Texture2D
	{
	public:
		OpenGlTexture(const std::string& path);
		virtual ~OpenGlTexture();
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		std::string m_Path;
		uint32_t m_Width,m_Height;
		uint32_t m_RenderID;

	};
}
