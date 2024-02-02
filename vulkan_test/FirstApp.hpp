#pragma once
#include "Lve_Window.hpp"
#include "Lve_Pipeline.hpp"
#include "Lve_Device.hpp"

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH  = 1024;
		static constexpr int HEIGHT = 768;

		FirstApp(const std::string& path) : lvePipeline{ lveDevice, path + "simple_shader.vert.spv", path + "simple_shader.frag.spv", LvePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)}
		{}
		void run();

	private:
		LveWindow lveWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		LveDevice lveDevice{ lveWindow };
		LvePipeline lvePipeline;

	};
}