#pragma once
#ifdef BSS_PLATFORM_WINDOW

extern BSS::Application* BSS::CreateApplication();

int main(int argc, char** argv)
{
	auto app = BSS::CreateApplication();
	app->Run();
	delete app;
}
#endif // BSS_PLATFORM_WINDOW
