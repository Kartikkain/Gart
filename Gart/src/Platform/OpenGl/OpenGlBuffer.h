#include "Renderer/Buffer.h"

namespace Gart
{
	class OpenGlVertexBuffer : public VertexBuffer
	{
	public:
		OpenGlVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGlVertexBuffer();
		virtual void Bind() const;
		virtual void UnBind() const;
	private:
		uint32_t m_RenderID;
	};

	class OpenGlIndexBuffer : public IndexBuffer
	{
	public:
		OpenGlIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGlIndexBuffer();
		virtual void Bind() const;
		virtual void UnBind() const;
		virtual uint32_t GetCount() const;
	private:
		uint32_t m_RenderID;
		uint32_t m_Count;
	};
}
