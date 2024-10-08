#pragma once

#include"BSS/Core.h"

#include <string>
#include <glm/glm.hpp>
namespace Gart {

	class Shader 
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual const std::string& GetName() const = 0;
		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name,const std::string& vertexsrc, const std::string& fragmentsrc);

	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name,const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filePath);
		Ref<Shader> Load(const std::string& name, const std::string& filePath);

		Ref<Shader> Get(const std::string& name);
	private:

		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};

}
