workspace "Gart"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


include "Dependencies.lua"


include "Gart-ScriptCore"
include "Gart/vendor/GLFW"
include "Gart/vendor/Box2D"
include "Gart/vendor/Glad"
include "Gart/vendor/msdf-atlas-gen"
include "Gart/vendor/ImGui"
include "Gart/vendor/yaml-cpp"

project "Gart"
	location "Gart"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir("bin/".. OutputDir .."/%{prj.name}")
	objdir("bin-int/".. OutputDir .."/%{prj.name}")

	pchheader "bsspch.h"
	pchsource "Gart/src/bsspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
		
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.Filewatcher}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.VulkanSDK}"
	}

	links
	{
		"GLFW",
		"Box2D",
		"Glad",
		"imgui",
		"yaml-cpp",
		"msdf-atlas-gen",
		"opengl32.lib",
		"%{Library.mono}",
		"%{Library.WinSock}",
		"%{Library.WinMM}",
		"%{Library.WinVersion}",
		"%{Library.Bcrypt}"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system.windows"
		
		systemversion "latest"

		defines
		{
			"BSS_PLATFORM_WINDOW",
			"BSS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		links
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.Bcrypt}"
		}

		

	filter "configurations:Debug"
		defines {"BSS_PLATFORM_WINDOW","BSS_BUILD_DLL","BSS_DEBUG"}
		buildoptions "/MDd"
		symbols "on"

		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "BSS_RELEASE"
		buildoptions "/MD"
		optimize "on"

		links 
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "BSS_DIST"
		buildoptions "/MD"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}


project "Gart-Editor"
	location "Gart-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"Gart/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.Filewatcher}",
		"%{IncludeDir.ImGuizmo}"
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