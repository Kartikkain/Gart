#include "bsspch.h"
#include "OpenGlVertexArray.h"
#include<glad/glad.h>

namespace Gart
{

	static GLenum ShaderDataTypeToOpenGlBaseType(Gart::ShaderDataType type)
	{

		switch (type)
		{
			case Gart::ShaderDataType::Float:   return GL_FLOAT;
			case Gart::ShaderDataType::Float2:  return GL_FLOAT;
			case Gart::ShaderDataType::Float3:  return GL_FLOAT;
			case Gart::ShaderDataType::Float4:  return GL_FLOAT;
			case Gart::ShaderDataType::Mat3:    return GL_FLOAT;
			case Gart::ShaderDataType::Mat4:    return GL_FLOAT;
			case Gart::ShaderDataType::Int:     return GL_INT;
			case Gart::ShaderDataType::Int2:    return GL_INT;
			case Gart::ShaderDataType::Int3:    return GL_INT;
			case Gart::ShaderDataType::Int4:    return GL_INT;
			case Gart::ShaderDataType::Bool:    return GL_INT;
		}
		return 0;
	}

	OpenGlVertexArray::OpenGlVertexArray()
	{
		glCreateVertexArrays(1, &m_RenderID);
	}
	void OpenGlVertexArray::Bind() const
	{
		glBindVertexArray(m_RenderID);
	}

	void OpenGlVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGlVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{

		glBindVertexArray(m_RenderID);
		vertexBuffer->Bind();

		BufferLayout m_layout = vertexBuffer->GetLayout();

		uint32_t index = 0;
		for (const auto& element : m_layout)
		{

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetElementCount(),
				ShaderDataTypeToOpenGlBaseType(element.Type),
				element.Normalize ? GL_TRUE : GL_FALSE,
				m_layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);

	}

	void OpenGlVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RenderID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}