local DependencyFetcher = require "dependency_fetcher" 
DependencyFetcher.Setup()  -- Check and fetch dependencies before anything else

outputDirectory = "../../Binaries/%{cfg.system}-%{cfg.architecture}/"
intermediateDirectory = "../../Intermediate/%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

workspace "CritEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"ReleaseDev",
		"Release"
	}

project "CritEngine"
	location "CritEngine"
	kind "SharedLib"
	language "C++"
	
	targetdir (outputDirectory)
	objdir (intermediateDirectory .. "%{prj.name}")
	includedirs { "CritEngine/thirdparty/libs/glfw/include/", "CritEngine/thirdparty/libs/glad/include/", "CritEngine/thirdparty/libs/glm/", "CritEngine/thirdparty/libs/imgui/", "CritEngine/thirdparty/libs/imgui/examples"}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	links { "GLFW", "GLM", "GLAD", "ImGui" }

	filter "system:windows"
		cppdialect "C++23"
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

	include "CritEngine/thirdparty/glfw.lua"
	include "CritEngine/thirdparty/glad.lua"
	include "CritEngine/thirdparty/glm.lua"
	include "CritEngine/thirdparty/imgui.lua"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	

	targetdir (outputDirectory)
	objdir (intermediateDirectory .. "%{prj.name}")
	

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"%{wks.location}/CritEngine/src",
		"CritEngine/thirdparty/libs/glm/",
		"CritEngine/thirdparty/libs/imgui/"
	}
	
	links 
	{
		"CritEngine", "GLM", "ImGui"
	}

	filter "system:windows"
		cppdialect "C++23"
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
