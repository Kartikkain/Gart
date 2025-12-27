#pragma once

#undef INFINITE
#include "msdf-atlas-gen.h"

namespace Gart
{
	struct MSDFData
	{
		std::vector<msdf_atlas::GlyphGeometry> m_Glyph;
		msdf_atlas::FontGeometry m_FontGeometry;
	};
}