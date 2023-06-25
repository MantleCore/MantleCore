project "MantleGraphics"
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
            "include/**.h",
            "include/**.cpp",
            "src/**.h",
            "src/**.cpp",
            "tests/**.h",
            "tests/**.cpp"
        }
    
        links { 
            "$(VULKAN_SDK)/lib/vulkan-1.lib",
        }
    
        includedirs { 
            "$(VULKAN_SDK)/include",
            "$(ProjectDir)include",
            "$(ProjectDir)tests"
        }

    filter "configurations:Debug"
        buildoptions "/MTd"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        buildoptions "/MT"
        runtime "Release"
        optimize "On"