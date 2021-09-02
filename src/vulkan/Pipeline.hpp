/*
*   Modified version of the framework provided by Brendan Galea in his Vulkan
*   tutorial series (https://github.com/blurrypiano/littleVulkanEngine) 
*   Copyright (c) 2020 Brendan Galea
*/

#pragma once

#include <Device.hpp>

// std
#include <string>
#include <vector>

namespace vkr {

struct PipelineSet;

struct PipelineConfigInfo {
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class Pipeline {
   public:
    Pipeline(Device& device);
    ~Pipeline();

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    void bind(VkCommandBuffer commandBuffer);

    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
    static PipelineSet createGraphicsPipelines(
        Device& device,
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo);

   private:
    static std::vector<char> readFile(const std::string& filepath);

    static void createShaderModule(Device& device, const std::vector<char>& code, VkShaderModule* shaderModule);
    static void createShaderStageInfo(VkShaderModule& vertShader, VkShaderModule& fragShader,
                                      VkPipelineShaderStageCreateInfo shaderStages[]);

    Device& device;
    VkPipeline graphicsPipeline{nullptr};
    VkShaderModule vertShaderModule{nullptr};
    VkShaderModule fragShaderModule{nullptr};
};

struct PipelineSet {
    std::shared_ptr<Pipeline> meshes;
    std::shared_ptr<Pipeline> hair;
};

}  // namespace vkr
