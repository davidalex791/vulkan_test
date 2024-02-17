#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Lve_Window.hpp"
#include "Lve_Pipeline.hpp"
#include "Lve_Device.hpp"
#include "Lve_SwapChain.hpp"
#include "Lve_Model.hpp"
#include "Lve_GameObject.hpp"

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 1024;
		static constexpr int HEIGHT = 768;

		FirstApp(const std::string& path);
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:
		void loadGameObjects();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderGameObjects(VkCommandBuffer commandBuffer);

		LveWindow lveWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		LveDevice lveDevice{ lveWindow };
		std::unique_ptr<LvePipeline>lvePipeline;
		std::unique_ptr<LveSwapChain> lveSwapChain;
		VkPipelineLayout pipelineLayout; 
		std::vector<VkCommandBuffer> commandBuffers;
		std::string shadersPath;

		std::vector<LveGameObject> gameObjects;
	};
}