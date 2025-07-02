#pragma once
#ifdef BSS_PLATFORM_WINDOW

extern BSS::Application* BSS::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	BSS::Log::init();
	GART_PROFILE_BEGIN_SESSION("StartUp", "GartProfile-StartUp.json");
	auto app = BSS::CreateApplication({argc,argv});
	GART_PROFILE_END_SESSION();

	GART_PROFILE_BEGIN_SESSION("Runtime", "GartProfile-Runtime.json");
	app->Run();
	GART_PROFILE_END_SESSION();

	GART_PROFILE_BEGIN_SESSION("Delete", "GartProfile-Delete.json");
	delete app;
	GART_PROFILE_END_SESSION();
}
#endif // BSS_PLATFORM_WINDOW
