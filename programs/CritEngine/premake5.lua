local DependencyFetcher = require "dependency_fetcher" 
-- Check and fetch dependencies before anything else

local external_dir = "external/libs/"

print("Running premake5.lua")

print("\n[DependencyFetcher] Fetching dependencies...")

DependencyFetcher.FetchRepo("https://github.com/Dav1dde/glad.git", external_dir .. "glad", "c", "5bf3eda6da606324999775b88a90ed572202be93")
DependencyFetcher.FetchRepo("https://github.com/glfw/glfw.git", external_dir .. "glfw", "master", "e7ea71be039836da3a98cea55ae5569cb5eb885c")
DependencyFetcher.FetchRepo("https://github.com/g-truc/glm.git", external_dir .. "glm", "master", "a40974fb8647717b15dbfd212b4a33845a405a88") 
DependencyFetcher.FetchRepo("https://github.com/ocornut/imgui.git", external_dir .. "imgui", "master", "89b5a2c3d50e4ca6a0a88378f096d7d05ff1c962")
DependencyFetcher.FetchRepo("https://github.com/veselink1/refl-cpp.git", external_dir .. "refl-cpp", "master", "27fbd7d2e6d86bc135b87beef6b5f7ce53afd4fc")

print("[DependencyFetcher] Dependency fetch completed!\n")

workspace "CritEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"ReleaseDev",
		"Release"
	}

project "Core"
	location "units/%{prj.name}"
	kind "SharedLib"
	language "C++"
	cppdialect "C++23"

	filter "system:windows"
		staticruntime "Off"
		systemversion "latest"

		defines 
		{
			"ENGINE_BUILD_DLL",
			"CE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CE_DEBUG"
		symbols "On"

	filter "configurations:ReleaseDev"
		defines "CE_RELEASEDEV"
		optimize "On"

	filter "configurations:Release"
		defines "CE_RELEASE"
		optimize "On"

	filter {}

	targetdir("units/%{prj.name}/build/bin/%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/")
	objdir("units/%{prj.name}/build/obj/%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/")

	files
	{
		"units/%{prj.name}/src/%{prj.name}/**.h",
		"units/%{prj.name}/src/%{prj.name}/**.cpp"
	}

	includedirs 
	{ 
		"external/libs/glfw/include/",
		"external/libs/glad/include/", 
		"external/libs/glm/", 
		"external/libs/imgui/",
		"external/libs/refl-cpp/include"
	}

	links { "GLFW", "GLM", "GLAD", "ImGui" }

	include "external/glfw.lua"
	include "external/glad.lua"
	include "external/glm.lua"
	include "external/imgui.lua"
	include "external/refl-cpp.lua"

project "Sandbox"
	location "units/%{prj.name}"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"

	filter "system:windows"
		staticruntime "Off"
		systemversion "latest"

		defines 
		{
			"CE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CE_DEBUG"
		symbols "On"

	filter "configurations:ReleaseDev"
		defines "CE_RELEASEDEV"
		optimize "On"

	filter "configurations:Release"
		defines "CE_RELEASE"
		optimize "On"
	
	filter {}

	postbuildcommands { "{COPYFILE} %[units/Core/build/bin/%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/Core.dll] %[units/Sandbox/build/bin/%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/Core.dll]" }

	targetdir("units/%{prj.name}/build/bin/%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/")
	objdir("units/%{prj.name}/build/obj/%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/")
	
	files
	{
		"units/%{prj.name}/src/%{prj.name}/**.h",
		"units/%{prj.name}/src/%{prj.name}/**.cpp",
	}

	includedirs 
	{
		"units/Core/src",
		"external/libs/glm/",
		"external/libs/imgui/",
		"external/libs/refl-cpp/include/"
	}

	links { "Core", "GLM", "ImGui" }

	include "external/glm.lua"
	include "external/imgui.lua"
	include "external/refl-cpp.lua"
