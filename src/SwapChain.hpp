/*
*   Modified version of the framework provided by Brendan Galea in his Vulkan
*   tutorial series (https://github.com/blurrypiano/littleVulkanEngine) 
*   Copyright (c) 2020 Brendan Galea
*/

#pragma once

#include "Device.hpp"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <memory>
#include <string>
#include <vector>

namespace vkr {

class SwapChain {
   public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    SwapChain(Device &deviceRef, VkExtent2D windowExtent);
    SwapChain(
        Device &deviceRef, VkExtent2D windowExtent, std::shared_ptr<SwapChain> previous);

    ~SwapChain();

    SwapChain(const SwapChain &) = delete;
    SwapChain &operator=(const SwapChain &) = delete;

    VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
    VkRenderPass getRenderPass() { return renderPass; }
    VkImageView getImageView(int index) { return swapChainImageViews[index]; }
    std::vector<VkImageView> getImageViews() { return swapChainImageViews; }
    size_t imageCount() { return swapChainImages.size(); }
    VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
    VkExtent2D getSwapChainExtent() { return swapChainExtent; }
    uint32_t width() { return swapChainExtent.width; }
    uint32_t height() { return swapChainExtent.height; }

    float extentAspectRatio() {
        return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
    }
    VkFormat findDepthFormat();

    VkResult acquireNextImage(uint32_t *imageIndex);
    VkResult submitCommandBuffers(const std::vector<VkCommandBuffer> &buffers, uint32_t *imageIndex);
    static VkImageView createImageView(Device &device, VkImage image, VkFormat format);

    bool compareSwapFormats(const SwapChain &swapChain) const {
        return swapChain.swapChainDepthFormat == swapChainDepthFormat &&
               swapChain.swapChainImageFormat == swapChainImageFormat;
    }

    void createFramebuffers(const std::vector<std::vector<VkImageView>>& attachments, std::vector<VkFramebuffer>& framebuffers, VkRenderPass renderPass);

   private:
    void init();
    void createSwapChain();
    void createImageViews();
    void createDepthResources();
    void createRenderPass();
    void createFramebuffers();
    void createSyncObjects();

    // Helper functions
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    VkFormat swapChainImageFormat;
    VkFormat swapChainDepthFormat;
    VkExtent2D swapChainExtent;

    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkRenderPass renderPass;

    std::vector<VkImage> depthImages;
    std::vector<VkDeviceMemory> depthImageMemories;
    std::vector<VkImageView> depthImageViews;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    Device &device;
    VkExtent2D windowExtent;

    VkSwapchainKHR swapChain;
    std::shared_ptr<SwapChain> oldSwapChain;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;
};

}  // namespace vkr
