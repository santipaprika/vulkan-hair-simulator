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

struct ShaderPaths {
    std::string vertFilepath;
    std::string fragFilepath;
};

struct VertexInputDescriptions {
    std::vector<VkVertexInputBindingDescription> bindingDescription;
    std::vector<VkVertexInputAttributeDescription> attributeDescription;
};

struct PipelineConfigInfo {
    // PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    // PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

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

    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo, Device& device);
    static std::unique_ptr<PipelineSet> createGraphicsPipelines(
        Device& device,
        const std::vector<ShaderPaths>& shadersFilepaths,
        const std::vector<PipelineConfigInfo>& configInfo,
        std::vector<VertexInputDescriptions>& vertexInputDescriptions);

   private:
    static std::vector<char> readFile(const std::string& filepath);

    static void createShaderModule(Device& device, const std::vector<char>& code, VkShaderModule* shaderModule);
    static void createShaderStageInfo(VkShaderModule& vertShader, VkShaderModule& fragShader,
                                      std::vector<VkPipelineShaderStageCreateInfo> &shaderStages);

    Device& device;
    VkPipeline graphicsPipeline{nullptr};
    VkShaderModule vertShaderModule{nullptr};
    VkShaderModule fragShaderModule{nullptr};
};

struct PipelineSet {
    std::shared_ptr<Pipeline> meshes;
    std::shared_ptr<Pipeline> hair;

    PipelineSet(std::shared_ptr<Pipeline> meshes, std::shared_ptr<Pipeline> hair) {
        this->meshes = meshes;
        this->hair = hair;
    }
};

}  // namespace vkr
