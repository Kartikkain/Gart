#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"
namespace BSS
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1920, 1080);
		BSS_CORE_INFO(e);
		while (true)
		{

		}
	}
}
