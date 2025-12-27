#include "bsspch.h"
#include "Font.h"
#include "Core/Log.h"
#undef INFINITE
#include "msdf-atlas-gen.h"
#include "msdfgen.h"
#include "FontGeometry.h"
#include "GlyphGeometry.h"

#include "MSDFData.h"

namespace Gart
{
	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S,N> GenFunc>
	static Ref<Texture2D> CreateAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyph, const msdf_atlas::FontGeometry& fontGeometry,uint32_t width, uint32_t height)
	{
		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		generator.setAttributes(attributes);
		generator.setThreadCount(8);
		generator.generate(glyph.data(), (int)glyph.size());

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T,N>)generator.atlasStorage();

		TextureSpecifications l_FontTexSpecs;
		l_FontTexSpecs.m_width = bitmap.width;
		l_FontTexSpecs.m_height = bitmap.height;
		l_FontTexSpecs.m_imageFormat = ImageFormat::RGB8;
		l_FontTexSpecs.GenerateMinMap = false;

		Ref<Texture2D> texture = Texture2D::Create(l_FontTexSpecs);
		texture->SetData((void*)bitmap.pixels, bitmap.width * bitmap.height * 3);
		return texture;
	}


	Font::Font(const std::filesystem::path& font)
		:m_Data(new MSDFData())
	{
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		BSS_CORE_ASSERT(ft,"Couldn't able to Initialize the free type handle.");
		
		std::string filestring = font.string();
		msdfgen::FontHandle* l_font = msdfgen::loadFont(ft, filestring.c_str());
		if (!l_font)
		{
			BSS_CORE_ERROR("No font were loaded from {0}.", font);
			return;
		}

		struct CharSetRange
		{
			uint32_t Begin, End;
		};

		static const CharSetRange characterSetRange[]
		{
			{0x0020,0x00FF}
		};

		msdf_atlas::Charset l_characterSet;

		for (CharSetRange range : characterSetRange)
		{
			for (uint32_t c = range.Begin;c <= range.End;c++)
				l_characterSet.add(c);
		}

		const double l_fontScale = 1.0f;

		m_Data->m_FontGeometry = msdf_atlas::FontGeometry(&m_Data->m_Glyph);
		int  l_loaded = m_Data->m_FontGeometry.loadCharset(l_font, l_fontScale, l_characterSet);

		BSS_CORE_INFO("{0} character's loaded from glyph out of {1}", l_loaded, l_characterSet.size());

		double emSize = 40.0f;
		msdf_atlas::TightAtlasPacker l_packer;
		l_packer.setPixelRange(2.0f);
		l_packer.setMiterLimit(1.0f);
		l_packer.setPadding(0.0f);
		l_packer.setScale(emSize);
		int remaining = l_packer.pack(m_Data->m_Glyph.data(), (int)m_Data->m_Glyph.size());

		BSS_CORE_ASSERT(remaining == 0,"Not able to pack the glyph.");
		int l_width, l_height;
		l_packer.getDimensions(l_width, l_height);

		m_FontTex = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Test",(float)emSize, m_Data->m_Glyph, m_Data->m_FontGeometry, l_width, l_height);

		msdfgen::destroyFont(l_font);
		msdfgen::deinitializeFreetype(ft);
	}

	Font::~Font()
	{
		delete m_Data;
	}

	Ref<Font> Font::GetDefault()
	{
		static Ref<Font> DefaultFont;

		if (!DefaultFont)
			DefaultFont = std::make_shared<Font>("C:\\Windows\\Fonts\\segoesc.ttf");

		return DefaultFont;

	}
}