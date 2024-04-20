#pragma once

#include "Lve_Device.hpp"
#include <string.h>
#include <vulkan/vulkan_core.h>

namespace lve {
    class Lve_Texture {
    public:
        Lve_Texture(LveDevice &device, const std::string &filepath);
        ~Lve_Texture();

        Lve_Texture(const Lve_Texture &) = delete;
        Lve_Texture &operator=(const Lve_Texture &) = delete;
        Lve_Texture(Lve_Texture &&) = delete;
        Lve_Texture &operator=(Lve_Texture &&) = delete;

        VkSampler getSampler() { return sampler; }
        VkImageView getImageView() { return imageView; }
        VkImageLayout getImageLayout() { return imageLayout; }
    private:
        void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
        void generateMipmaps();

        int width, height, mipLevels;

        LveDevice& lveDevice;
        VkImage image;
        VkDeviceMemory imageMemory;
        VkImageView imageView;
        VkSampler sampler;
        VkFormat imageFormat;
        VkImageLayout imageLayout;
    };
}