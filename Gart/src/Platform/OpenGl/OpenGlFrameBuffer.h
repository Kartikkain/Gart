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
	private:

		

		uint32_t m_RendererID, m_colorAttachment, m_depthAttachment;

		FrameBufferSpecification m_specification;
	};
}