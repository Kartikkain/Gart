#include "bsspch.h"
#include "Shader.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "Renderer.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Gart {
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGl:return new OpenGLShader(filepath);
		}
		return nullptr;
	}
	Shader* Shader::Create(const std::string& vertexsrc, const std::string& fragmentsrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGl:return new OpenGLShader(vertexsrc, fragmentsrc);
		}
		return nullptr;
	}

}