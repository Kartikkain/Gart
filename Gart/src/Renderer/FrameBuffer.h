#pragma once

#include "Core/Core.h"

namespace Gart
{

	struct FrameBufferSpecification
	{
		uint32_t Width, Height;

		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public :
		virtual ~FrameBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual const uint32_t GetColorAttachmetID() const = 0;
		virtual const FrameBufferSpecification& GetFrameBufferSpecifier() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
		
	};
}
