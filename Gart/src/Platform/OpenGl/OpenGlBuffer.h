#include "Renderer/Buffer.h"

namespace Gart
{
	class OpenGlVertexBuffer : public VertexBuffer
	{
	public:
		OpenGlVertexBuffer(uint32_t size);
		OpenGlVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGlVertexBuffer();
		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		virtual void SetData(const void* data, uint32_t size) override;

	private:
		uint32_t m_RenderID;
		BufferLayout m_Layout;
	};

	class OpenGlIndexBuffer : public IndexBuffer
	{
	public:

		OpenGlIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGlIndexBuffer();
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual uint32_t GetCount() const override;

	private:
		uint32_t m_RenderID;
		uint32_t m_Count;
	};
}
