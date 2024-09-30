#pragma once

#include"Renderer/Shader.h"

namespace Gart
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexsrc, const std::string& fragmentsrc);
		virtual ~OpenGLShader();

		void Bind() const override;
		void UnBind() const override;
		
		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RenderID;
	};
}
