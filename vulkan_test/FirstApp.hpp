#pragma once
#include "Lve_Window.hpp"
#include "Lve_Pipeline.hpp"

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH  = 1024;
		static constexpr int HEIGHT = 768;

		FirstApp(const std::string& path) : lvePipeline{ path + "simple_shader.vert.spv", path + "simple_shader.frag.spv" } 
		{}
		void run();

	private:
		LveWindow lveWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		LvePipeline lvePipeline;

	};
}