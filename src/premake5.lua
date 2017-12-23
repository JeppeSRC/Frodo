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
        "/Zl",
        "/Gm",
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

workspace("Frodo")
    location "../solution/"
 
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
            links {
                "DXGI",
                "D3D11",
                "D3D10"
            }

        filter("configurations:Debug-DX")
            defines {"FD_DEBUG", "FD_DX" }
            setupDebugConfiguration()
            links {
                "DXGI",
                "D3D11",
                "D3D10"
            }
    end

    filter("configurations:Release-VK")
        defines {"FD_RELEASE", "FD_VK" }
        setupReleaseConfiguration()
        links {
            "vulkan-1"
        }

    filter("configurations:Debug-VK")
        defines {"FD_DEBUG", "FD_VK" }
        setupDebugConfiguration()
        links {
            "vulkan-1"
        }

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
        "Frodo-core/"
    }

    filter {"Release-VK or Debug-VK", "files:Frodo-core/**dx*.cpp"}
        flags "ExcludeFromBuild"

    filter("Release-VK or Debug-VK")
        excludes "Frodo-core/**dx*.*"

    filter {"Release-DX or Debug-DX", "files:Frodo-core/**vk*.cpp"}
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

    includedirs {"Frodo-core/", "Sandbox/"}

    links {"Frodo-core"}