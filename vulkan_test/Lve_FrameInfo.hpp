#pragma once
#include "Lve_Camera.hpp"

//lib
#include <vulkan/vulkan.h>

namespace lve
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		LveCamera &camera;
		VkDescriptorSet globalDescriptorSet;
	};


} //namespace lve
