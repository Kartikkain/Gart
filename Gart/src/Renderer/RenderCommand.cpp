#include "bsspch.h"
#include "RenderCommand.h"


namespace Gart
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGlRendererAPI();
}