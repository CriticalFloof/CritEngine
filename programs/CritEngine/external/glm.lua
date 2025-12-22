local c = require("common")

print("Running glm.lua")

project "GLM"
	location "libs/glm"
	kind "StaticLib"
	language "C"
	architecture "x86_64"

	targetdir(c.build_directory)
	objdir(c.object_directory)
	
	includedirs { "libs/glm/" }

	files
	{
		"libs/glm/glm/**"
	}
    
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		defines
		{
			"_GLM_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 
			"_GLM_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"