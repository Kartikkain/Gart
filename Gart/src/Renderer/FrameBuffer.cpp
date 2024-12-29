#include "bsspch.h"
#include "FrameBuffer.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGl/OpenGlFrameBuffer.h"

namespace Gart 
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGl: return std::make_shared<OpenGLFrameBuffer>(spec);
		}
		return nullptr;
	}

}