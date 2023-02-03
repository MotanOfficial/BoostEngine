include "./vendor/premake/premake_customization/solution_items.lua"
workspace "BoostEngine"
    architecture "x64"
    startproject "BoostEngine"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    solution_items
    {
        ".editorconfig"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["nlohmann"] = "%{wks.location}/BoostEngine/vendor/nlohmann"
IncludeDir["lunalogger"] = "%{wks.location}/BoostEngine/vendor/lunalogger"



group "Core"
    include "BoostEngine"
group ""
	include "vendor/premake"
group ""
