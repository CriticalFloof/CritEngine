project "ReflCpp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	architecture "x86_64"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"

	includedirs { "libs/refl-cpp/include/" }

	files
	{
		"libs/refl-cpp/include/*.cpp",
		"libs/refl-cpp/include/*.hpp",
	}

	systemversion "latest"
	staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"