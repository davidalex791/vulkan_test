#pragma once
#include "Lve_Camera.hpp"
#include "Lve_GameObject.hpp"

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
		LveGameObject::Map &gameObjects;
	};


} //namespace lve
