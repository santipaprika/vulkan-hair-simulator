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
    RenderSystem(Device &device, VkRenderPass renderPass, std::shared_ptr<Texture> texture);
    ~RenderSystem();

    RenderSystem(const RenderSystem &) = delete;
    RenderSystem &operator=(const RenderSystem &) = delete;

    void renderEntities(
        VkCommandBuffer commandBuffer,
        std::vector<Entity> &entities,
        const Camera &camera);

    void loadTexture(std::shared_ptr<Texture> texture);

   private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);
    
    void createDescriptorSetLayout();
    void createUniformBuffers();

    void createTextureImageView(VkImage TextureImage);
    void createTextureSampler();
    
    void createDescriptorSets();
    void createDescriptorPool();

    Device &device;

    std::unique_ptr<Pipeline> pipeline;

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    VkImageView textureImageView;
    VkSampler textureSampler;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
};
}  // namespace vkr
