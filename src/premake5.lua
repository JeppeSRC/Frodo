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
        "Debug",
        "Release-VK",
        "Debug-VK"
    }

    if (__TARGET_OS == "windows") then
        configurations {
            "Release-DX",
            "Debug-DX"
        }
    end

    includedirs "Frodo-core/"

    -- Global
    floatingpoint "Fast"
    intrinsics "on"

    architecture "x86_64"
    defines "FD_PLATFORM_X64"

    callingconvention "FastCall"

    filter {"Release or Release-VK or Release-DX"}
        optimize "Speed"
        inlining "Auto"

    filter {"Debug or Debug-VK or Debug-DX"}
        optimize "Off"
        inlining "Disabled"       

    -- Windows Specific
    filter("system:windows")
        defines {
            "FD_WINDOWS",
            "_CRT_NON_CONFORMING_SWPRINTFS",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter {"system:windows", "Release or Release-VK or Release-DX"}
        buildoptions {
            "/GL",
            "/sdl-",
            "/Ot",
            "/GS-",
            "/arch:AVX2"
        }

        linkoptions {
            "/LTCG:incremental"
        }

    filter {"system:windows", "Debug or Debug-VK or Debug-DX"}
        buildoptions {
            "/sdl",
            "/arch:AVX2"
        }

    -- Linux Specific
    filter("system:linux")
        defines {
            "FD_LINUX"
        }

    filter {"system:linux", "Release or Release-VK"}
        buildoptions {
            "-msse4.1",
            "-mfma",
            "-mavx2",
            "-fpermissive"
        }

    filter {"system:linux", "Debug or Debug-VK"}
        buildoptions {
            "-msse4.1",
            "-mfma",
            "-mavx2",
            "-fpermissive"
        }

    filter ""

    targetdir "../bin/%{cfg.buildcfg}/%{cfg.platform}/"
    objdir "../bin/%{cfg.buildcfg}/%{cfg.platform}/intermediates"

project("Frodo-core")
    kind "StaticLib"
    location "../solution/Frodo-core/"

    files {
        "Frodo-core/**.cpp",
        "Frodo-core/**.h",
        "Frodo-core/**.c"
    }

    filter {"system:linux"}
        removefiles {
            "Frodo-core/**DX*.*",
            "Frodo-core/**WIN*.*"
        }
    
    filter {"system:windows"}
        removefiles {
            "Frodo-core/**LNX*.*"
        }

    filter {"system:windows", "Release-VK or Debug-VK", "files:Frodo-core/**DX*.cpp"}
        flags "ExcludeFromBuild"

    filter {"Release-DX or Debug-DX", "files:Frodo-core/**VK*.cpp"} 
        flags "ExcludeFromBuild"

    filter ""

project("Sandbox")
    kind "ConsoleApp"
    location "../solution/Sandbox"
    dependson "Frodo-core"

    files {
        "Sandbox/**.cpp",
        "Sandbox/**.h"
    }

    includedirs "Sandbox/"

    links "Frodo-core"

    filter {"system:windows"}
        postbuildcommands { "call \"$(SolutionDir)../src/post.bat\" \"$(SolutionDir)../src/Sandbox/res\"" }

    filter {"system:windows", "Release or Debug or Release-DX or Debug-DX"}
        links {
            "DXGI",
            "D3D11"
        }