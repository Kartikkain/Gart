#pragma once

#include "Renderer/FrameBuffer.h"

namespace Gart
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();
		void Invalidate();
		virtual void Bind() override;
		virtual void Unbind() override;
		virtual const uint32_t GetColorAttachmetID() const override { return m_colorAttachment; }
		virtual const FrameBufferSpecification& GetFrameBufferSpecifier() const override { return m_specification; }
		virtual void Resize(uint32_t width, uint32_t height) override;
	private:

		

		uint32_t m_RendererID = 0, m_colorAttachment = 0, m_depthAttachment = 0;

		FrameBufferSpecification m_specification;
	};
}