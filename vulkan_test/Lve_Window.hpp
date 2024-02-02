#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

namespace lve
{
	class LveWindow
	{
	public:
		LveWindow(int width, int height, const std::string &name);
		~LveWindow();

		
		bool ShouldClose() 
		{
			return glfwWindowShouldClose(window);
		}
		void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

	private:
		LveWindow(const LveWindow&) = delete;
		LveWindow &operator=(const LveWindow&) = delete;

		void initWindow();

		const int width;
		const int height;
		std::string windowName;
		GLFWwindow* window{ nullptr };
	};
}//namespace lve