#include "Lve_Window.hpp"
#include <stdexcept>

namespace lve
{
	LveWindow::LveWindow(int width_, int height_, const std::string &name_) : width{ width_ }, height{ height_ }, windowName{ name_ }
	{
		initWindow();
	}
	
	LveWindow::~LveWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void LveWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		if (nullptr == window)
		{
			throw std::runtime_error("Could not create window.");
		}
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (VK_SUCCESS != glfwCreateWindowSurface(instance, window, nullptr, surface))
		{
			throw std::runtime_error("Could not create window surface");
		}
	}

	void LveWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto lveWindow = reinterpret_cast<LveWindow*>(glfwGetWindowUserPointer(window));
		lveWindow->framebufferResized = true;
		lveWindow->width = width;
		lveWindow->height = height;
	}

} //namespace lve