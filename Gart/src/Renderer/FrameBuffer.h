#pragma once

#include "Core/Core.h"

namespace Gart
{
	enum FrameBufferTextureFormat
	{
		None = 0,
		RGBA8,
		Depth24Stencil8,
		Depth = Depth24Stencil8
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;

		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
		:TextureFormat(format){}

		FrameBufferTextureFormat TextureFormat;

	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;

		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachment)
		:Attachments(attachment){}

		std::vector<FrameBufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		FrameBufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public :
		virtual ~FrameBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual const uint32_t GetColorAttachmetID(uint32_t index = 0) const = 0;
		virtual const FrameBufferSpecification& GetFrameBufferSpecifier() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
		
	};
}
