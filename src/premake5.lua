function setupReleaseConfiguration()
    buildoptions {
        "/GL",
        "/SDL-",
        "/Ot",
        "/GS-",
        "/arch:SSE2",
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
        "/SDL",
        "/Zl",
        "/Gm",
        "/arch:IA32"
    }

    optimize "Off"
    inlining "Disabled"
end

function setupX64Platform()
    architecture "x86_64"
    defines "FD_PLATFORM_X64"
    removebuildoptions "/arch:SSE2"

    callingconvention "FastCall"
end

function setupX86Platform()
    architecture "x86"
    defines "FD_PLATFORM_X86"
    removebuildoptions "/arch:AVX2"
end


workspace("Frodo")
    location "../solution/"
    language "c++"
    cppdialect "c++14"
 
    configurations {
        "Release-VK",
        "Debug-VK"
    }

    platforms {
        "x86",
        "x64"
    }

    defines "FD_LINUX"

    floatingpoint "Fast"

    if _ARGS[1] == "Windows" then
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

    filter("platforms:x86")
        setupX86Platform()

    filter("platforms:x64")
        setupX64Platform()

project("Frodo-core")
    kind("StaticLib")
    location "../solution/Frodo-core/"
    
    files {
        "Frodo-core/**.cpp",
        "Frodo-core/**.c",
        "Frodo-core/**.h"
    }

    targetdir "../bin/$(Configuration)/$(Platform)/"
    objdir "../bin/intermediates"
  

    includedirs {
        "Frodo-core/"
    }

project("Sandbox")
    kind("ConsoleApp")
    location "../solution/Sandbox"
    dependson "Frodo-core"

    files {
        "Sandbox/**.cpp",
        "Sandbox/**.c",
        "Sandbox/**.h"
    }

    includedirs {"Frodo-core/", "Sandbox/"}

    links {"Frodo-core"}