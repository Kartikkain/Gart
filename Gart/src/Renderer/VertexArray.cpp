#include "bsspch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGlVertexArray.h"
namespace Gart
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGl: return new OpenGlVertexArray();
		}
		return nullptr;
	}

}