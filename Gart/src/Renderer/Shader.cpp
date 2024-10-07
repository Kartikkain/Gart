#include "bsspch.h"
#include "Shader.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "Renderer.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Gart {
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGl:return std::make_shared<OpenGLShader>(filepath);
		}
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& name,const std::string& vertexsrc, const std::string& fragmentsrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGl:return std::make_shared<OpenGLShader>(name,vertexsrc, fragmentsrc);
		}
		return nullptr;
	}


	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		m_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		return m_shaders[name];
	}


}