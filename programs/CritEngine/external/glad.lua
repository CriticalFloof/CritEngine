local c = require("common")

print("Running glad.lua")

project "GLAD"
	location "libs/glad"
	kind "StaticLib"
	language "C"
	architecture "x86_64"

	targetdir(c.build_directory)
	objdir(c.object_directory)
    
    includedirs { "libs/glad/include/" }

	files { "libs/glad/src/glad.c" }
    
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		defines
		{
			"_GLAD_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 
			"_GLAD_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"