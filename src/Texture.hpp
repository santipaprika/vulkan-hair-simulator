#pragma once

#include "Device.hpp"

// img
#include <stb_image.h>

// libs
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace vkr {
class Texture {
   public:
    struct Builder {
        stbi_uc *pixels;
        int texWidth, texHeight, texChannels;

        void loadImage(const std::string &filepath);
    };

    Texture(Device &device, const Builder &builder);
    ~Texture();

    void destroy();

    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    void createImage(const Builder &builder, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);
    static std::unique_ptr<Texture> createTextureFromFile(Device &device, const std::string &filepath);
    VkImage getTextureImage() { return textureImage; }
    const VkDescriptorImageInfo &getDescriptorInfo() { return descriptorInfo; }

    void createTextureImageView();
    void createTextureSampler();

   private:
    Device &device;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkDescriptorImageInfo descriptorInfo;
};
}  // namespace vkr