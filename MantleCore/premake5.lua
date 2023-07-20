project "MantleCore"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    defines {
        "WINDOWS"
        --later os supports
    }

    filter "system:windows"
        systemversion "latest"
        targetdir("../bin/" .. outputdir .. "/%{prj.name}")
        objdir("../bin-int/" .. outputdir .. "/%{prj.name}")
    
        files {
            "MantleInclude/**.h",

            "MantleCommon/**.h",
            "MantleCommon/**.cpp",

            "MantleGfx/**.h",
            "MantleGfx/**.cpp",
            
            "MantleAudio/**.h",
            "MantleAudio/**.cpp",

            "MantleMemory/**.h",
            "MantleMemory/**.cpp",

            "MantleWindow/**.h",
            "MantleWindow/**.cpp",

            "MantleConsole/**.h",
            "MantleConsole/**.cpp",

            "MantleTests/**.h",
            "MantleTests/**.cpp"
        }
    
        links { 
            "$(VULKAN_SDK)/lib/vulkan-1.lib"
        }
    
        includedirs { 
            "$(VULKAN_SDK)/include",

            "$(ProjectDir)MantleInclude/",
            "$(ProjectDir)MantleCommon/",
            "$(ProjectDir)MantleGfx/",
            "$(ProjectDir)MantleAudio/",
            "$(ProjectDir)MantleMemory/",
            "$(ProjectDir)MantleWindow/",
            "$(ProjectDir)MantleConsole/",
            "$(ProjectDir)MantleTests/",
        }

    filter "configurations:Debug"
        buildoptions "/MTd"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        buildoptions "/MT"
        runtime "Release"
        optimize "On"