#pragma once

#include <string>
#include <glm/glm.hpp>
namespace Gart {

	class Shader 
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		static Shader* Create(const std::string& filepath);
		static Shader* Create(const std::string& vertexsrc, const std::string& fragmentsrc);

	};

}
