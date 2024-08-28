#include "bsspch.h"
#include "Platform/OpenGl/OpenGlBuffer.h"
#include "Renderer.h"

namespace Gart
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGl: return new OpenGlVertexBuffer(vertices, size);
		}

		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGl: return new OpenGlIndexBuffer(indices, size);
		}

		return nullptr;
	}
}