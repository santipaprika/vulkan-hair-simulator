/*
*   Modified version of the framework provided by Brendan Galea in his Vulkan
*   tutorial series (https://github.com/blurrypiano/littleVulkanEngine) 
*   Copyright (c) 2020 Brendan Galea
*/

#pragma once

#include <Camera.hpp>
#include <Device.hpp>
#include <Entity.hpp>
#include <Pipeline.hpp>

// std
#include <memory>
#include <vector>

namespace vkr {

struct PoolSize {
    VkDescriptorType type;
    uint32_t descriptorCount;
};

class RenderSystem {
   public:
    RenderSystem(Device &device, VkRenderPass renderPass, std::vector<Entity> &entities);
    ~RenderSystem();

    RenderSystem(const RenderSystem &) = delete;
    RenderSystem &operator=(const RenderSystem &) = delete;

    void setupDescriptors();

    void renderEntities(
        VkCommandBuffer commandBuffer,
        const Camera &camera);

   private:
    void createDescriptorSetLayout();
    void createUniformBuffers();
    void createDescriptorPool(const std::vector<PoolSize> &poolSizes, int maxSets);
    void createDescriptorSets();

    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    Device &device;

    // std::unique_ptr<Pipeline> pipeline;
    std::unique_ptr<PipelineSet> pipelines;

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;

    std::vector<Entity> &entities;

    VkDescriptorPool descriptorPool;

    void *data;
};
}  // namespace vkr
