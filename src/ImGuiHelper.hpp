#pragma once

#include "Application.hpp"

namespace vkr {

class ImGuiHelper {
   public:
    ImGuiHelper(Application &application);
    ~ImGuiHelper();
    
    VkCommandBuffer renderImGui();
    void updateImGuiFramebuffers();

   private:
    void setupImGuiContext();
    void createImGuiDescriptorPool();
    void createImGuiRenderPass();

    Application &application;

    VkDescriptorPool imGuiDescriptorPool;

    VkRenderPass imGuiRenderPass;

    VkCommandPool imGuiCommandPool;
    std::vector<VkCommandBuffer> imGuiCommandBuffers;
    std::vector<VkFramebuffer> imGuiFramebuffers;
};

}  // namespace vkr
