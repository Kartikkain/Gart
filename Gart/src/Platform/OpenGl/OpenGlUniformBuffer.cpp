#include "bsspch.h"
#include "OpenGlUniformBuffer.h"

#include <glad/glad.h>

namespace Gart
{
	OpenGlUniformBuffer::OpenGlUniformBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &RenderID);
		glNamedBufferData(RenderID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, RenderID);
	}
	OpenGlUniformBuffer::~OpenGlUniformBuffer()
	{
		glDeleteBuffers(1, &RenderID);
	}
	void OpenGlUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(RenderID, offset, size, data);
	}
}