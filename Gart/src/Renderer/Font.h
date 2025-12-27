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

		 MSDFData* GetMSDFData() const { return m_Data; }

		 Ref<Texture2D> GetFontTexture() const { return m_FontTex; }

		 static Ref<Font> GetDefault();
	private:

		MSDFData* m_Data;
		Ref<Texture2D> m_FontTex;

	};

	
}
