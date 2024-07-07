#pragma once

namespace BSS
{
	class GraphicContext
	{
	public:

		virtual void init() = 0;
		virtual void swapbuffers() = 0;
	};
}