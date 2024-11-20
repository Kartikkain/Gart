#pragma once

#include<glm/glm.hpp> 
#include "VertexArray.h"

namespace Gart
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGl = 1
		};
		
	public:
		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t Width, uint32_t Height) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }

	private:

		static API s_API;

	};

	
}