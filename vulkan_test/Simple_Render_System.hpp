#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Lve_Pipeline.hpp"
#include "Lve_Device.hpp"
#include "Lve_GameObject.hpp"

namespace lve
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(LveDevice &device, VkRenderPass renderPass, std::string shadersPath);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<LveGameObject> &gameObjects);


	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		LveDevice &lveDevice;

		std::unique_ptr<LvePipeline>lvePipeline;
		VkPipelineLayout pipelineLayout;

		std::string shadersPath;
	};
}