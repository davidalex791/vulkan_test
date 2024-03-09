echo "Compiling shaders..."
C:\VulkanSDK\1.3.275.0\Bin\glslc.exe shaders\simple_shader.vert -o ..\x64\Debug\simple_shader.vert.spv
C:\VulkanSDK\1.3.275.0\Bin\glslc.exe shaders\simple_shader.frag -o ..\x64\Debug\simple_shader.frag.spv
echo "done"
pause