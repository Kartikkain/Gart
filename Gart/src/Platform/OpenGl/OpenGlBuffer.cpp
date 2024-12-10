#include "bsspch.h"
#include "OpenGlBuffer.h"
#include <glad/glad.h>

namespace Gart
{
	// Vertex Buffer Implimentation.

	OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size)
	{
		GART_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGlVertexBuffer::OpenGlVertexBuffer(float* vertices, uint32_t size)
	{
		GART_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	OpenGlVertexBuffer::~OpenGlVertexBuffer()
	{
		GART_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RenderID);
	}
	void OpenGlVertexBuffer::Bind() const
	{
		GART_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
	}
	void OpenGlVertexBuffer::UnBind() const
	{
		GART_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGlVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	


	// IndexBuffer Implimenation.

	OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t* indices, uint32_t count)
		:m_Count(count)
	{
		GART_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	OpenGlIndexBuffer::~OpenGlIndexBuffer()
	{
		GART_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RenderID);
	}
	void OpenGlIndexBuffer::Bind() const
	{
		GART_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
	}
	void OpenGlIndexBuffer::UnBind() const
	{
		GART_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	uint32_t OpenGlIndexBuffer::GetCount() const
	{
		return m_Count;
	}
}