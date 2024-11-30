#include "bsspch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGlTexture.h"
namespace Gart
{
	Ref<Texture2D> Texture2D::Create(uint32_t Width,  uint32_t Height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGl: return std::make_shared<OpenGlTexture>(Width,Height);
		}

		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGl: return std::make_shared<OpenGlTexture>(path);
		}

		return nullptr;
	}

}