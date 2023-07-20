--sln name
workspace "MantleCore"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

    --start at the project folder
    startproject "MantleCore"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--include project folder
include "MantleCore"


---------------------------------------------------------------------------------------
--custom actions

newaction {
    trigger = "clean",
    description = "Remove all binaries and intermediate binaries, and vs files.",
    execute = function()
        print("Removing binaries")
        os.rmdir("./bin")
        print("Removing intermediate binaries")
        os.rmdir("./bin-int")
        print("Removing project files")
        os.rmdir("./.vs")
        os.rmdir("./.vscode")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        print("Done")
    end
}
    