#include "bsspch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGlVertexArray.h"
namespace Gart
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGl: return std::make_shared<OpenGlVertexArray>();
		}
		return nullptr;
	}

}