echo "Compiling shaders..."

C:\VulkanSDK\1.3.275.0\Bin\glslc.exe shaders\simple_shader.vert -o %1\simple_shader.vert.spv
C:\VulkanSDK\1.3.275.0\Bin\glslc.exe shaders\simple_shader.frag -o %1\simple_shader.frag.spv
C:\VulkanSDK\1.3.275.0\Bin\glslc.exe shaders\point_light.vert -o %1\point_light.vert.spv
C:\VulkanSDK\1.3.275.0\Bin\glslc.exe shaders\point_light.frag -o %1\point_light.frag.spv

echo "done"
