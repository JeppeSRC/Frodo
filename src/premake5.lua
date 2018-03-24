function setupReleaseConfiguration()
    if _TARGET_OS == "windows" then
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
    elseif _TARGET_OS == "linux" then
        buildoptions {
            "-msse4.1",
            "-mfma"
        }
    end

    optimize "Speed"
    inlining "Auto"
end

function setupDebugConfiguration()
    if _TARGET_OS == "windows" then
        buildoptions {
            "/sdl",
            "/arch:AVX2"
        }
    elseif _TARGET_OS == "linux" then
        buildoptions {
            "-msse4.1",
            "-mfma"
        }
    end

    optimize "Off"
    inlining "Disabled"
end

function setupX64Platform()
    architecture "x86_64"
    defines "FD_PLATFORM_X64"

    callingconvention "FastCall"
end

vk_path = os.getenv("VULKAN_SDK");

if (vk_path == nil) then
    vk_path = os.getenv("VK_SDK_PATH")
end

if (vk_path == nil) then
    print("No vulkan sdk path. Set environment variable VULKAN_SDK or VK_SDK_PATH to the vulkan sdk directory")
    os.exit()
 end

workspace("Frodo")
    location "../solution/"
    cppdialect "c++14"
    startproject "Sandbox"

    configurations {
        "Release-VK",
        "Debug-VK"
    }

    platforms {
        "x64"
    }

    defines "FD_LINUX"

    floatingpoint "Fast"
    intrinsics "on"

    if _TARGET_OS == "windows" then
        removedefines "FD_LINUX"
        defines { 
            "FD_WINDOWS",
            "_CRT_NON_CONFORMING_SWPRINTFS",
            "_CRT_SECURE_NO_WARNINGS"
        }

        configurations {
            "Release-DX",
            "Debug-DX",
        }

        buildoptions {
            "/wd4251"
        }

        filter("configurations:Release-DX")
            defines {"FD_RELEASE", "FD_DX" }
            setupReleaseConfiguration()

        filter("configurations:Debug-DX")
            defines {"FD_DEBUG", "FD_DX" }
            setupDebugConfiguration()
    end

    filter("configurations:Release-VK")
        defines {"FD_RELEASE", "FD_VK" }
        setupReleaseConfiguration()

    filter("configurations:Debug-VK")
        defines {"FD_DEBUG", "FD_VK" }
        setupDebugConfiguration()

    filter("platforms:x64")
        setupX64Platform()

project("Frodo-core")
    kind("StaticLib")
    cppdialect "c++14"
    location "../solution/Frodo-core/"
    
    files {
        "Frodo-core/**.cpp",
        "Frodo-core/**.h",
        "Frodo-core/**.c"
    }

    targetdir "../bin/%{cfg.buildcfg}/%{cfg.platform}/"
    objdir "../bin/intermediates"
  

    includedirs {
        "Frodo-core/"
    }

    filter("Release-VK or Debug-VK")
        includedirs { vk_path .. "/include/vulkan" }
        libdirs { vk_path ..  "/Lib" }
        targetprefix "VK-"

    filter("Release-DX or Debug-DX")
        targetprefix "DX-"

    filter {"Release-VK or Debug-VK", "files:Frodo-core/**DX*.cpp"}
        flags "ExcludeFromBuild"

    filter {"Release-DX or Debug-DX", "files:Frodo-core/**VK*.cpp"}
        flags "ExcludeFromBuild"

    if _TARGET_OS == "linux" then
        removefiles {
            "Frodo-core/platforms/windows/**.*",
            "Frodo-core/**dx*.*"
        }

    elseif _TARGET_OS == "windows" then
        filter {"system:windows"}
            removefiles "Frodo-core/platforms/linux/**.*"
    end

project("Sandbox")
    kind("ConsoleApp")
    location "../solution/Sandbox"
    dependson "Frodo-core"

    targetdir "../bin/%{cfg.buildcfg}/%{cfg.platform}/"
    objdir "../bin/intermediates"

    files {
        "Sandbox/**.cpp",
        "Sandbox/**.h"
    }
    
    filter("Release-VK or Debug-VK")
        libdirs { vk_path ..  "/Lib" }
        includedirs { vk_path .. "/include/vulkan" }

    filter {}

    includedirs {"Frodo-core/", "Sandbox/"}

    links {"Frodo-core"}

    if _TARGET_OS == "windows" then

        postbuildcommands { "call \"$(SolutionDir)../src/post.bat\" \"$(SolutionDir)../src/Sandbox/res\"" }
   
        filter("Release-DX or Release-VK")
            links {
                "D3D11",
                "DXGI"
            }
    end

    filter("Release-VK or Debug-VK")
        links {
            "vulkan-1"
        }