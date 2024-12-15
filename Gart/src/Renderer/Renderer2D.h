#pragma once

#include "OrthoGraphicCamera.h"
#include "Texture.h"

namespace Gart
{
	class Renderer2D
	{
	public:
		static void init();
		static void Shutdown();

		static void BeginScene(const OrthoGraphicCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, float tilling = 1.0f, glm::vec4 tintcolor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, float tilling = 1.0f, glm::vec4 tintcolor = glm::vec4(1.0f));

		static void DrawRotateQuad(const glm::vec2& position, float rotation,  const glm::vec2& size, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D> texture, float tilling = 1.0f, glm::vec4 tintcolor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D> texture, float tilling = 1.0f, glm::vec4 tintcolor = glm::vec4(1.0f));

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCounts = 0;

			uint32_t GetNumbersOfVertices() { return QuadCounts * 4; }
			uint32_t GetNumbersOfIndices() { return QuadCounts * 6; }
		};
		
		static void ResetStats();
		static Statistics GetStats();

	private:

		static void FlushAndRestart();
	};
}