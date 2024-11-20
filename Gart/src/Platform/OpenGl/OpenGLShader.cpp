#include "bsspch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace Gart {
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
		return 0;
	}
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSource = PreProcess(source);
		Compile(shaderSource);

		// Get FileName.
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		int count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count); 
	}
	OpenGLShader::OpenGLShader(const std::string& name,const std::string& vertexsrc, const std::string& fragmentsrc)
		:m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shadersource;
		shadersource[GL_VERTEX_SHADER] = vertexsrc;
		shadersource[GL_FRAGMENT_SHADER] = fragmentsrc;

		Compile(shadersource);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RenderID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RenderID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLuint location = glGetUniformLocation(m_RenderID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLuint location = glGetUniformLocation(m_RenderID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, glm::vec2& value)
	{
		GLuint location = glGetUniformLocation(m_RenderID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, glm::vec3& value)
	{
		GLuint location = glGetUniformLocation(m_RenderID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, glm::vec4& value)
	{
		GLuint location = glGetUniformLocation(m_RenderID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLuint location = glGetUniformLocation(m_RenderID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLuint location = glGetUniformLocation(m_RenderID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSource;

		const char* typeToken = "#type";
		size_t typeTokenLenght = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			size_t begin = pos + typeTokenLenght + 1;
			std::string type = source.substr(begin, eol - begin);
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSource[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSource;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		std::array<GLenum,2> glShadersID;
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			std::string& source = kv.second;
			GLuint shader = glCreateShader(type);


			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCstr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceCstr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.

				// In this simple program, we'll just leave
				break;
			}

			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShadersID[glShaderIDIndex++] = shader;
		}


		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShadersID)
			{
				glDeleteShader(id);
			}

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShadersID)
		{
			glDetachShader(program, id);
		}

		m_RenderID = program;

	}

}