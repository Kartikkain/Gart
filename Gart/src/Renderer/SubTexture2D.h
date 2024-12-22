#pragma once

#include "Texture.h"
#include <glm/glm.hpp>
namespace Gart
{
	class SubTexture2D
	{
	public:

		SubTexture2D(Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D>& GetTexture() const { return m_Texture; }
		const glm::vec2* GetTextCoords() const { return m_TexCoords; }

		static Ref<SubTexture2D> CreateFromCoords(Ref<Texture2D>& texture, const glm::vec2& coords,const glm::vec2& cellSize ,const glm::vec2& spriteSize = {1,1});

	private:
		Ref<Texture2D> m_Texture;
		
		glm::vec2 m_TexCoords[4];

	};
}
