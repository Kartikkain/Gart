#include "bsspch.h"
#include "Renderer.h"

namespace Gart
{

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexarray)
	{
		vertexarray->Bind();
		RenderCommand::DrawIndexed(vertexarray);
	}

}