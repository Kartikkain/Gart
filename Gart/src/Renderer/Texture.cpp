#include "bsspch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGlTexture.h"
namespace Gart
{
	
	Ref<Texture2D> Texture2D::Create(const TextureSpecifications& specs, Buffer data)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGl: return std::make_shared<OpenGlTexture>(specs,data);
		}

		return nullptr;
	}

}