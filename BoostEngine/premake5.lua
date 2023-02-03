project "BoostEngine"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ccpch.h"
	pchsource "src/ccpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/**.h",
		"vendor/**.cpp",
		"vendor/**.hpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.nlohmann}",
		"%{IncludeDir.lunalogger}"
	}

	links
	{
		
	}

	
	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "CC_DEBUG"
		characterset "MBCS"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CC_RELEASE"
		characterset "MBCS"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CC_DIST"
		characterset "MBCS"
		runtime "Release"
		optimize "on"