#include "bsspch.h"
#include "Font.h"
#undef INFINITE
#include "msdf-atlas-gen.h"
#include "msdfgen.h"

namespace Gart
{
	Font::Font(const std::filesystem::path& font)
	{
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		if (ft)
		{
			std::string filestring = font.string();
			msdfgen::FontHandle* font = msdfgen::loadFont(ft, filestring.c_str());
			if (font)
			{
				msdfgen::Shape shape;
				if (msdfgen::loadGlyph(shape, font, 'K'))
				{
					shape.normalize();
					msdfgen::edgeColoringSimple(shape, 3.0);
					msdfgen::Bitmap<float, 3> msdf(32, 32);

					msdfgen::generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));
					msdfgen::savePng(msdf, "Output.png");
				}
				msdfgen::destroyFont(font);
			}
			msdfgen::deinitializeFreetype(ft);
		}
	}
}