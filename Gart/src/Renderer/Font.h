#pragma once
#include <filesystem>
#include "Core/Core.h"
#include "Renderer/Texture.h"

namespace Gart
{
	struct MSDFData;
	class  Font
	{
	public:
		 Font(const std::filesystem::path& font);
		 ~Font();
		 Ref<Texture2D> GetFontTexture() const { return m_FontTex; }
	private:

		MSDFData* m_Data;
		Ref<Texture2D> m_FontTex;

	};

	
}
