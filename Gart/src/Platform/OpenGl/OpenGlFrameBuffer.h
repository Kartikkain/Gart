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
		virtual const uint32_t GetColorAttachmetID(uint32_t index) const override { return  m_ColorAttachments[index]; }
		virtual int ReadPixel(uint32_t attachment, int x, int y) override;
		virtual const FrameBufferSpecification& GetFrameBufferSpecifier() const override { return m_specification; }
		virtual void Resize(uint32_t width, uint32_t height) override;
	private:

		

		uint32_t m_RendererID = 0;
		uint32_t m_colorAttachment = 0, m_depthAttachment = 0;
		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecification;
		FrameBufferTextureSpecification m_DepthAttachmentSpecification = FrameBufferTextureFormat::None;
		FrameBufferSpecification m_specification;
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}