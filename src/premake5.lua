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
            "-mfma",
            "-mavx2",
            "-fpermissive"
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
            "-mfma",
            "-mavx2",
            "-fpermissive"
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
        if _TARGET_OS == "windows" then
            libdirs { vk_path ..  "/Lib" }
        end
        if _TARGET_OS == "linux" then
            libdirs { vk_path ..  "/lib" }
        end
        targetprefix "VK-"

    filter("Release-DX or Debug-DX")
        targetprefix "DX-"

    filter {"Release-VK or Debug-VK", "files:Frodo-core/**DX*.cpp"}
        flags "ExcludeFromBuild"

    filter {"Release-DX or Debug-DX", "files:Frodo-core/**VK*.cpp"}
        flags "ExcludeFromBuild"

    filter {"system:linux"}
        removefiles {
            "Frodo-core/platforms/windows/**.h",
            "Frodo-core/platforms/windows/**.cpp",
            "Frodo-core/platforms/directx/**.h",
            "Frodo-core/platforms/directx/**.cpp"
        }

    filter {"system:windows"}
        removefiles {
              "Frodo-core/platforms/linux/**.h",
               "Frodo-core/platforms/linux/**.cpp"
        }


    filter ""

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

    includedirs {"Frodo-core/", "Sandbox/"}

    links {"Frodo-core"}
    
    filter {"Release-VK or Debug-VK", "system:windows"}
        libdirs { vk_path ..  "/Lib" }
        includedirs { vk_path .. "/include/vulkan" }
        links "vulkan-1"

    filter {"Release-VK or Debug-VK", "system:linux"}
        libdirs { vk_path ..  "/lib" }
        includedirs { vk_path .. "/include/vulkan" }
        links {
            "vulkan",
            "X11",
            "xcb"
        }

    filter ""
        
        if _TARGET_OS == "windows" then

        postbuildcommands { "call \"$(SolutionDir)../src/post.bat\" \"$(SolutionDir)../src/Sandbox/res\"" }
   
        filter ("Release-DX or Release-VK")
            links {
                "D3D11",
                "DXGI"
            }
    end

    filter ""