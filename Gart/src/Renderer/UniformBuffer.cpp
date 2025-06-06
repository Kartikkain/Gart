#include "bsspch.h"
#include "UniformBuffer.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGl/OpenGlUniformBuffer.h"
namespace Gart
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGl: std::make_shared<OpenGlUniformBuffer>(size, binding);
		}

		BSS_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}