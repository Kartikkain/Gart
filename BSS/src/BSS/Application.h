#pragma once

#include "Core.h"

#include "Events/Event.h"

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

