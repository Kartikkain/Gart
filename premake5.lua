workspace "Gart"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


IncludeDir = {}
IncludeDir["GLFW"] = "Gart/vendor/GLFW/include"
IncludeDir["Glad"] = "Gart/vendor/Glad/include"
IncludeDir["ImGui"] = "Gart/vendor/imgui"
IncludeDir["glm"] = "Gart/vendor/glm"

include "Gart/vendor/GLFW"
include "Gart/vendor/Glad"
include "Gart/vendor/ImGui"

project "Gart"
	location "Gart"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/".. OutputDir .."/%{prj.name}")
	objdir("bin-int/".. OutputDir .."/%{prj.name}")

	pchheader "bsspch.h"
	pchsource "Gart/src/bsspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"imgui",
		"opengl32.lib"
	}

	filter "system.windows"
		
		systemversion "latest"

		defines
		{
			"BSS_PLATFORM_WINDOW",
			"BSS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
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
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/".. OutputDir .."/%{prj.name}")
	objdir("bin-int/".. OutputDir .."/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Gart/vendor/spdlog/include",
		"Gart/src",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Gart"
	}

	filter "system.windows"
		cppdialect "C++17"
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