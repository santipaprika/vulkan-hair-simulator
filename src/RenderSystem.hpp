/*
*   Modified version of the framework provided by Brendan Galea in his Vulkan
*   tutorial series (https://github.com/blurrypiano/littleVulkanEngine) 
*   Copyright (c) 2020 Brendan Galea
*/

#pragma once

#include "Camera.hpp"
#include "Device.hpp"
#include "Entity.hpp"
#include "Pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace vkr {

const std::string root_path = "..\\..\\";

class RenderSystem {
   public:
    RenderSystem(Device &device, VkRenderPass renderPass);
    ~RenderSystem();

    RenderSystem(const RenderSystem &) = delete;
    RenderSystem &operator=(const RenderSystem &) = delete;

    void renderEntities(
        VkCommandBuffer commandBuffer,
        std::vector<Entity> &entities,
        const Camera &camera);

   private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    Device &device;

    std::unique_ptr<Pipeline> pipeline;
    VkPipelineLayout pipelineLayout;
};
}  // namespace vkr
