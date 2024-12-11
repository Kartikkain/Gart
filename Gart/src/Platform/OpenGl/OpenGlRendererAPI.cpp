#include "bsspch.h"
#include "OpenGlRendererAPI.h"
#include <glad/glad.h>
namespace Gart
{
	void OpenGlRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	void OpenGlRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGlRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t Width, uint32_t Height)
	{
		glViewport(x, y, Width, Height);
	}

	void OpenGlRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGlRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
	{
		uint32_t l_count = count ? vertexArray->GetIndexBuffer()->GetCount() : count;
		glDrawElements(GL_TRIANGLES, l_count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}