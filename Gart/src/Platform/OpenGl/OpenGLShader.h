#pragma once

#include"Renderer/Shader.h"
// TODO: Remove later.
typedef unsigned int GLenum;
namespace Gart
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name,const std::string& vertexsrc, const std::string& fragmentsrc);
		virtual ~OpenGLShader();

		void Bind() const override;
		void UnBind() const override;

		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
		virtual void SetFloat4(const std::string& name, glm::vec4& value) override;
		virtual void SetFloat3(const std::string& name, glm::vec3& value) override;

		const std::string& GetName() const override { return m_Name; }
		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32_t m_RenderID;
		std::string m_Name;
	};
}
