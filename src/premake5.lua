vk_path = os.getenv("VULKAN_SDK");

if (vk_path == nil) then
    vk_path = os.getenv("VK_SDK_PATH")
end

if (vk_path == nil) then
    print("No vulkan sdk path. Set environment variable VULKAN_SDK or VK_SDK_PATH to the vulkan sdk directory")
    os.exit()
 end

 workspace("Frodo")
    startproject "Sandbox"
    location "../solution/"
    cppdialect "c++14"

    platforms "x64"

    configurations {
        "Release",
        "Debug"
    }

    includedirs {
        "Frodo-core/",
        vk_path .. "/Include/vulkan/"
    }
    -- Global
    floatingpoint "Fast"
    intrinsics "on"

    architecture "x86_64"
    defines "FD_PLATFORM_X64"

    callingconvention "FastCall"

    flags {
        "MultiProcessorCompile"
    }

    filter {"Release"}
        optimize "Speed"
        inlining "Auto"
        defines "FD_RELEASE"

    filter {"Debug"}
        optimize "Off"
        inlining "Disabled"
        defines "FD_DEBUG"

    -- Windows Specific
    filter("system:windows")
        defines {
            "FD_WINDOWS",
            "_CRT_NON_CONFORMING_SWPRINTFS",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter {"system:windows", "Release"}
        buildoptions {
            "/GL",
            "/Ot",
            --"/GS-",
            "/arch:AVX2"
        }

        flags {
            "NoBufferSecurityCheck"
        }

        linkoptions {
            "/LTCG:incremental"
        }

    filter {"system:windows", "Debug"}
        buildoptions {
            "/sdl",
            "/arch:AVX2"
        }

    -- Linux Specific
    filter("system:linux")
        defines {
            "FD_LINUX"
        }

    filter {"system:linux", "Release"}
        buildoptions {
            "-msse4.1",
            "-mfma",
            "-mavx2",
            "-fpermissive"
        }

    filter {"system:linux", "Debug"}
        buildoptions {
            "-msse4.1",
            "-mfma",
            "-mavx2",
            "-fpermissive"
        }

    filter ""

project("Frodo-core")
    kind "StaticLib"
    location "../solution/Frodo-core/"

    targetdir "%{sln.location}/../bin/%{cfg.buildcfg}/Frodo-core/"
    objdir "%{sln.location}/../bin/%{cfg.buildcfg}/intermediates/Frodo-core/"

    files {
        "Frodo-core/**.cpp",
        "Frodo-core/**.h",
        "Frodo-core/**.c"
    }

    filter {"system:linux"}
        removefiles {
            "Frodo-core/**WIN*.*"
        }
    
    filter {"system:windows"}
        removefiles {
            "Frodo-core/**LNX*.*"
        }

    filter ""

project("Sandbox")
    kind "ConsoleApp"
    location "../solution/Sandbox"
    dependson "Frodo-core"

    targetdir "%{sln.location}/../bin/%{cfg.buildcfg}/Sandbox/"
    objdir "%{sln.location}/../bin/%{cfg.buildcfg}/intermediates/Sandbox/"

    files {
        "Sandbox/**.cpp",
        "Sandbox/**.h"
    }

    includedirs {
        "Sandbox/"
    }
    
    libdirs { vk_path .. "/Lib/" }

    links "Frodo-core"
    links "vulkan-1"

    filter {"system:windows"}
        postbuildcommands { "call \"$(SolutionDir)../src/post.bat\" \"$(SolutionDir)../src/Sandbox/res\"" }
