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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		if (nullptr == window)
		{
			throw std::runtime_error("Could not create window.");
		}
	}
} //namespace lve