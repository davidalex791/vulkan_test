#include "FirstApp.hpp"

namespace lve
{
	void FirstApp::run()
	{
		while (!lveWindow.ShouldClose())
		{
			glfwPollEvents();
		}
	}
}