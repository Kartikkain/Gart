#pragma once

#include "Core.h"

namespace BSS
{
	class BSS_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	Application* CreateApplication();
}

