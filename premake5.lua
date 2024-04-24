workspace "BSS"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


IncludeDir = {}
IncludeDir["GLFW"] = "BSS/vendor/GLFW/include"
IncludeDir["Glad"] = "BSS/vendor/Glad/include"

include "BSS/vendor/GLFW"
include "BSS/vendor/Glad"

project "BSS"
	location "BSS"
	kind "SharedLib"
	language "C++"

	targetdir("bin/".. OutputDir .."/%{prj.name}")
	objdir("bin-int/".. OutputDir .."/%{prj.name}")

	pchheader "bsspch.h"
	pchsource "BSS/src/bsspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system.windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		defines
		{
			"BSS_PLATFORM_WINDOW",
			"BSS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. OutputDir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines {"BSS_PLATFORM_WINDOW","BSS_BUILD_DLL","BSS_DEBUG"}
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "BSS_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "BSS_DIST"
		buildoptions "/MD"
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
		defines {"BSS_PLATFORM_WINDOW","BSS_DEBUG"}
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "BSS_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "BSS_DIST"
		buildoptions "/MD"
		optimize "on"