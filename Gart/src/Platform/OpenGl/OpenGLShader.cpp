#include "bsspch.h"
#include "Core/Log.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <fstream>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include "Core/Timer.h"

namespace Gart {

	namespace Utils
	{
		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex") return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
			BSS_CORE_ASSERT(false, "Unknown Shader type!`");
			return 0;
		}

		static shaderc_shader_kind GLShaderStageToShaderc(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			BSS_CORE_ASSERT(false, "Could not retrieve the stage to shaderc");
			return (shaderc_shader_kind)0;
		}

		static const char* ShaderCToString(shaderc_shader_kind shadercKind)
		{
			switch (shadercKind)
			{
			case shaderc_glsl_vertex_shader: return "Vertex";
			case shaderc_glsl_fragment_shader: return "Fragement";
			}
			BSS_CORE_ASSERT(false, "Could not retrieve the stage to shaderc");
			return nullptr;
		}

		static const char* GLShaderStageToShaderType(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return "VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "FRAGMENT_SHADER";
			}
			BSS_CORE_ASSERT(false, "Could not retrieve the stage to string");
			return nullptr;
		}
		static const char* GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			BSS_CORE_ASSERT(false, "Could not retrieve the stage to string");
			return nullptr;
		}

		static const char* GetCacheDirectory()
		{
			return "assets/cache/shader/opengl";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER: return ".cached_opengl.frag";
			}
			BSS_CORE_ASSERT(false, "Could not retrieved opengl cache extention");

			return "";
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER: return ".cached_vulkan.frag";
			}
			BSS_CORE_ASSERT(false, "Could not retrieved vulkan cache extention");

			return "";
		}

		// Debug Functions
		static size_t CheckForNullCharacters(const std::string& str)
		{
			size_t count = 0;
			for (size_t i=0;i<str.size();++i)
			{
				if (str[i] == '\0') ++count;
			}
			return count;

		}

		static void RemoveNullCharacter(std::string& str)
		{
			str.erase(std::remove(str.begin(),str.end(),'\0'), str.end());
		}
	}
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
		BSS_CORE_ASSERT(false, "Unknown Shader type!`");
		return 0;
	}
	OpenGLShader::OpenGLShader(const std::string& filepath)
		:m_FilePath(filepath)
	{
		GART_PROFILE_FUNCTION();

		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(filepath);
		if(Utils::CheckForNullCharacters(source) > 0) Utils::RemoveNullCharacter(source);

		auto shaderSource = PreProcess(source);
		
		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSource);
			CompileOrGetOpenGlBinaries();
			CreateProgram();
			
			BSS_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}
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
		GART_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shadersource;
		shadersource[GL_VERTEX_SHADER] = vertexsrc;
		shadersource[GL_FRAGMENT_SHADER] = fragmentsrc;

		CompileOrGetVulkanBinaries(shadersource);
		CompileOrGetOpenGlBinaries();
		CreateProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		GART_PROFILE_FUNCTION();

		glDeleteProgram(m_RenderID);
	}

	void OpenGLShader::Bind() const
	{
		GART_PROFILE_FUNCTION();

		glUseProgram(m_RenderID);
	}

	void OpenGLShader::UnBind() const
	{
		GART_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		GART_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		GART_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, glm::vec4& value)
	{
		GART_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, glm::vec3& value)
	{
		GART_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		GART_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* value, uint32_t count)
	{
		UploadUniformIntArray(name, value, count);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLuint location = glGetUniformLocation(m_RenderID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* value, uint32_t count)
	{
		GLuint location = glGetUniformLocation(m_RenderID, name.c_str());
		glUniform1iv(location, count, value);
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
		GART_PROFILE_FUNCTION();

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
		else
		{
			BSS_CORE_ERROR("Could not open file {0}", filePath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		GART_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSource;

		const char* typeToken = "#type";
		size_t typeTokenLenght = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			BSS_CORE_ASSERT(eol != std::string::npos, "Syntax error: Missing end of line after #type");
			size_t begin = pos + typeTokenLenght + 1;
			std::string type = source.substr(begin, eol - begin);
			BSS_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Inavlid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			BSS_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error: No shader code found after #type");
			pos = source.find(typeToken, nextLinePos);
			
			BSS_CORE_ASSERT(pos == std::string::npos || pos > nextLinePos,"Shader parsing error: next #type found before previous block ended");
			shaderSource[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSource;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GART_PROFILE_FUNCTION();

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
				BSS_CORE_ERROR("{0}", infoLog.data());
				BSS_CORE_ASSERT(false, "Shader Compilation Failure!");
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

			BSS_CORE_ERROR("{0}", infoLog.data());
			BSS_CORE_ASSERT(false, "Shader Link Failure!");
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

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSource)
	{
		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;


		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

		//options.SetGenerateDebugInfo();
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		std::filesystem::path cachedir = Utils::GetCacheDirectory();
		
		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();

		for (auto&& [stage, source] : shaderSource)
		{
			std::filesystem::path shaderfilepath = m_FilePath;
			
			std::filesystem::path cachePath = cachedir / (shaderfilepath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachePath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderc(stage), m_FilePath.c_str(), options);

				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					BSS_CORE_ERROR(module.GetErrorMessage());
					BSS_CORE_ASSERT(false, "compilation to vulkan binaries failed.");
				}

				shaderData[stage] = std::vector<uint32_t>(module.begin(), module.end());

				std::ofstream out(cachePath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
				
			}
		}
		
		
		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	void OpenGLShader::CompileOrGetOpenGlBinaries()
	{
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;


		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

		

		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		OpenGlShaderCode.clear();

		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			std::filesystem::path shaderPath = m_FilePath;
			std::filesystem::path cachePath = cacheDirectory / (shaderPath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachePath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size/ sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				
				spirv_cross::CompilerGLSL glslcompiler(spirv);
				OpenGlShaderCode[stage] = glslcompiler.compile();
				auto& source = OpenGlShaderCode[stage];

				ReflectInterfaceVariables(stage, spirv);

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderc(stage), m_FilePath.c_str());
				
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					BSS_CORE_ERROR(module.GetErrorMessage());
					BSS_CORE_ASSERT(false, "module compilation falied.");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachePath, std::ios::out | std::ios::binary);

				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;

		for (auto&& [stage, spirv] : m_OpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);
		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		
		if (isLinked == GL_FALSE)
		{
			GLint maxlength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxlength);

			std::vector<GLchar> infoLog(maxlength);
			glGetProgramInfoLog(program, maxlength, &maxlength, infoLog.data());
			BSS_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);

			return;
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RenderID = program;
	}


	auto GetGLSLTypeName = [&](const spirv_cross::SPIRType& type) -> std::string {
		std::string base;
		switch (type.basetype) {
		case spirv_cross::SPIRType::Float: base = "float"; break;
		case spirv_cross::SPIRType::Int: base = "int"; break;
		case spirv_cross::SPIRType::UInt: base = "uint"; break;
		case spirv_cross::SPIRType::Double: base = "double"; break;
		case spirv_cross::SPIRType::Boolean: base = "bool"; break;
		default: base = "unknown"; break;
		}

		if (type.columns > 1) {
			return base + std::to_string(type.columns) + "x" + std::to_string(type.vecsize);
		}
		else if (type.vecsize > 1) {
			return base + std::to_string(type.vecsize);
		}
		else {
			return base;
		}
		};
	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		BSS_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
		BSS_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		BSS_CORE_TRACE("    {0} resources", resources.sampled_images.size());
		BSS_CORE_TRACE("    {0} Intputs from this stage", resources.stage_inputs.size());
		BSS_CORE_TRACE("    {0} Outputs from this stage", resources.stage_outputs.size());

		if (stage == GL_VERTEX_SHADER || stage == GL_FRAGMENT_SHADER)
		{
			std::string title = stage == GL_VERTEX_SHADER ? "Vertex output" : "Fragment input";

			BSS_CORE_TRACE("-------- {0} --------", title);
			BSS_CORE_TRACE(" | {0} | {1} | {2} |", "Loc", "Name", "Type");
			BSS_CORE_TRACE("------------------------------------------------");

			const auto& InterfaceVars = stage == GL_VERTEX_SHADER ? resources.stage_outputs : resources.stage_inputs;

			for (const auto& var : InterfaceVars)
			{
				uint32_t location = compiler.get_decoration(var.id, spv::DecorationLocation);
				spirv_cross::SPIRType type = compiler.get_type(var.type_id);
				std::string typestr = GetGLSLTypeName(type);
				BSS_CORE_TRACE(" | {0} | {1} | {2} |", location, var.name.c_str(), typestr.c_str());
			}

			BSS_CORE_TRACE("------------------------------------------------");
		}

		BSS_CORE_TRACE("Uniform buffers:");

		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& buffertype = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(buffertype);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = buffertype.member_types.size();

			BSS_CORE_TRACE("  {0}", resource.name);
			BSS_CORE_TRACE("    Size = {0}", bufferSize);
			BSS_CORE_TRACE("    Binding = {0}", binding);
			BSS_CORE_TRACE("    Members = {0}", memberCount);
		}

	}

	void OpenGLShader::ReflectInterfaceVariables(GLenum stage, const std::vector<uint32_t>& spirv)
	{

		spirv_cross::CompilerGLSL compiler(spirv);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		auto stageStr = Utils::GLShaderStageToString(stage);
		BSS_CORE_TRACE("== {0} Interface Reflection ==", stageStr);

		if (!resources.stage_inputs.empty() && stage == GL_FRAGMENT_SHADER)
		{
			for (auto& input : resources.stage_inputs)
			{
				uint32_t location = compiler.get_decoration(input.id, spv::DecorationLocation);
				BSS_CORE_TRACE(" {0} | {1} | {2}", location, input.name, compiler.get_name(input.id));
			}
		}

		if (!resources.stage_outputs.empty() && stage == GL_VERTEX_SHADER)
		{
			for (auto& output : resources.stage_outputs)
			{
				uint32_t location = compiler.get_decoration(output.id, spv::DecorationLocation);
				BSS_CORE_TRACE(" {0} | {1} | {2}", location, output.name, compiler.get_name(output.id));
			}
		}


	}


	

}