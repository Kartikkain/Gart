workspace "BSS"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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

	includedirs
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

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..OutputDir.. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "BSS_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "BSS_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "BSS_DIST"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	

	targetdir("bin/".. OutputDir .."/%{prj.name}")
	objdir("bin-int/".. OutputDir .."/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"BSS/vendor/spdlog/include",
		"BSS/src"
	}

	links
	{
		"BSS"
	}

	filter "system.windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		defines
		{
			"BSS_PLATFORM_WINDOW"
		}

		
	filter "configurations:Debug"
		defines "BSS_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "BSS_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "BSS_DIST"
		optimize "on"