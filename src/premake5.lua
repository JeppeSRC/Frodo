function setupReleaseConfiguration()
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

    optimize "Speed"
    inlining "Auto"
end

function setupDebugConfiguration() 
    buildoptions {
        "/sdl",
        "/arch:AVX2"
    }

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
    location "../solution/Frodo-core/"
    
    files {
        "Frodo-core/**.cpp",
        "Frodo-core/**.h"
    }

    targetdir "../bin/$(Configuration)/$(Platform)/"
    objdir "../bin/intermediates"
  

    includedirs {
        "Frodo-core/",
    }

    filter("Release-VK or Debug-VK")
        includedirs { vk_path .. "/include/vulkan" }
        libdirs { vk_path ..  "/Bin" }

    filter {"Release-VK or Debug-VK", "files:Frodo-core/**DX*.cpp"}
        flags "ExcludeFromBuild"

    filter("Release-VK or Debug-VK")
        excludes "Frodo-core/**dx*.*"

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

    files {
        "Sandbox/**.cpp",
        "Sandbox/**.h"
    }

    filter("Release-VK or Debug-VK")
        libdirs { vk_path ..  "/Bin" }
        includedirs { vk_path .. "/include/vulkan" }

    filter {}

    includedirs {"Frodo-core/", "Sandbox/"}

    links {"Frodo-core"}

    if _TARGET_OS == "windows" then
   
        filter("configurations:Release-DX")
            links {
                "D3D11",
                "DXGI"
            }

        filter("configurations:Debug-DX")
            links {
               "D3D11",
               "DXGI"
            }
    end

    filter("configurations:Release-VK")
        links {
            "vulkan-1"
        }

    filter("configurations:Debug-VK")
        links {
            "vulkan-1"
        }
