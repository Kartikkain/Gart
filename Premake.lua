workspace "BSS"
	architecture "x64"

	configrations
	{
		"Debug",
		"Release",
		"Dist"
	}

OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.srchitecture}"

project "BSS"
	location "BSS"
	kind "SharedLib"
	language "C++"

	targetdir("bin/".. OutputDir .."/%{prj.name}")
	objdir("bin-int/".. OutputDir .."/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	include
	{
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system.windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		defines
		{
			"BSS_PLATFORM_WINDOW",
			"BSS_BUILD_DLL"
		}