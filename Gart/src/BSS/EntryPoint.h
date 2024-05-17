#pragma once
#ifdef BSS_PLATFORM_WINDOW

extern BSS::Application* BSS::CreateApplication();

int main(int argc, char** argv)
{
	BSS::Log::init();
	BSS_CORE_WARN("Initialize Log");
	BSS_CLIENT_INFO("Hello");
	auto app = BSS::CreateApplication();
	app->Run();
	BSS_CLIENT_INFO("Hello");
	delete app;
}
#endif // BSS_PLATFORM_WINDOW
