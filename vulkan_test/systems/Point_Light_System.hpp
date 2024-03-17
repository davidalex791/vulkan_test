#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Lve_Camera.hpp"
#include "Lve_Pipeline.hpp"
#include "Lve_Device.hpp"
#include "Lve_GameObject.hpp"
#include "Lve_FrameInfo.hpp"

namespace lve
{
	class PointLightSystem
	{
	public:
		PointLightSystem(LveDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, std::string shadersPath);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem &) = delete;
		PointLightSystem &operator=(const PointLightSystem &) = delete;

		void update(FrameInfo &frameInfo, GlobalUbo &ubo);
		void render(FrameInfo &frameInfo);

	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		LveDevice &lveDevice;

		std::unique_ptr<LvePipeline>lvePipeline;
		VkPipelineLayout pipelineLayout;

		std::string shadersPath;
	};
}