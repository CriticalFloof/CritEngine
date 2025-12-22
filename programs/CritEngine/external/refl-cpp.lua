local c = require("common")

print("Running refl-cpp.lua")

project "ReflCpp"
    location "libs/refl-cpp"
	kind "StaticLib"
	language "C++"
	architecture "x86_64"

	targetdir(c.build_directory)
	objdir(c.object_directory)

	includedirs { "libs/refl-cpp/include/" }

	files
	{
		"libs/refl-cpp/include/*.cpp",
        "libs/refl-cpp/include/*.hpp"
	}

	systemversion "latest"
	staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"