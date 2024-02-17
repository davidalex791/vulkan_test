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

		LveWindow(const LveWindow&) = delete;
		LveWindow& operator=(const LveWindow&) = delete;


		bool ShouldClose() 
		{
			return glfwWindowShouldClose(window);
		}
		void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

		bool wasWindowResized() 
		{ 
			return framebufferResized; 
		}
		void resetWindowResizedFlag() 
		{ 
			framebufferResized = false; 
		}

		VkExtent2D getExtent() 
		{ 
			return 	{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; 
		}

	private:
		static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
		void initWindow();

		int width;
		int height;
		bool framebufferResized{ false };
		
		std::string windowName;
		GLFWwindow* window{ nullptr };
	};
}//namespace lve