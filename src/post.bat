echo Running post build script

"%VULKAN_SDK%\Bin\glslangValidator.exe" -V %1%/test.frag -o res/frag.spv

"%VULKAN_SDK%\Bin\glslangValidator.exe" -V %1%/test.vert -o res/vert.spv