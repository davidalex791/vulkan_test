#pragma once
#include <memory>
#include <vector>
#include <string>
#include <cassert>

#include "Lve_Window.hpp"
#include "Lve_Device.hpp"
#include "Lve_SwapChain.hpp"
#include "Lve_Model.hpp"

namespace lve
{
	class LveRenderer
	{
	public:
		LveRenderer(LveWindow& window, LveDevice& device);
		~LveRenderer();

		LveRenderer(const LveRenderer&) = delete;
		LveRenderer& operator=(const LveRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const
		{
			return lveSwapChain->getRenderPass();
		};
		float getAspectRatio() const
		{
			return lveSwapChain->extentAspectRatio();
		}

		bool isFrameInProgress() const { return isFrameStarted; }
		VkCommandBuffer getCurrentCommandBuffer() const
		{
			assert(commandBuffers.size() > currentFrameIndex && "Current Frame Index out of command buffers size");
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const
		{
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		LveWindow &lveWindow;
		LveDevice &lveDevice;
		std::unique_ptr<LveSwapChain> lveSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted{ false };
	};
}