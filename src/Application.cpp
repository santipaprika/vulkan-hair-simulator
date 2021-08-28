/*
*   Modified version of the framework provided by Brendan Galea in his Vulkan
*   tutorial series (https://github.com/blurrypiano/littleVulkanEngine)
*   Copyright (c) 2020 Brendan Galea
*/

#include "Application.hpp"

#include "Camera.hpp"
#include "InputController.hpp"
#include "RenderSystem.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <iostream>
#include <stdexcept>

namespace vkr {

Application::Application() { loadEntities(); }

Application::~Application() {
    for (auto framebuffer : imGuiFramebuffers) {
        vkDestroyFramebuffer(device.device(), framebuffer, nullptr);
    }

    vkDestroyRenderPass(device.device(), imGuiRenderPass, nullptr);

    vkFreeCommandBuffers(device.device(), imGuiCommandPool, static_cast<uint32_t>(imGuiCommandBuffers.size()), imGuiCommandBuffers.data());
    vkDestroyCommandPool(device.device(), imGuiCommandPool, nullptr);

    vkDestroyDescriptorPool(device.device(), imGuiDescriptorPool, nullptr);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::run() {
    RenderSystem renderSystem{device, renderer.getSwapChainRenderPass(), entities};
    Camera camera{};

    auto viewerObject = Entity::createEntity();
    InputController cameraController{};

    setupImGuiContext();

    auto currentTime = std::chrono::high_resolution_clock::now();
    while (!window.shouldClose()) {
        glfwPollEvents();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = renderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

        if (auto commandBuffer = renderer.beginFrame()) {
            renderer.beginCommandBuffer(commandBuffer);
            renderer.beginSwapChainRenderPass(commandBuffer);

            renderSystem.renderEntities(commandBuffer, camera);

            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endCommandBuffer(commandBuffer);

            int frameIdx = renderer.getFrameIndex();
            int imageIdx = renderer.getImageIndex();
            renderer.beginCommandBuffer(imGuiCommandBuffers[frameIdx]);
            renderer.beginSwapChainRenderPass(imGuiCommandBuffers[frameIdx],
                                              imGuiRenderPass,
                                              imGuiFramebuffers[imageIdx],
                                              std::vector<VkClearValue>({{0.f, 0.f, 0.f, 1.f}}),
                                              false);

            // Record Imgui Draw Data and draw funcs into command buffer
            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), imGuiCommandBuffers[frameIdx]);

            renderer.endSwapChainRenderPass(imGuiCommandBuffers[frameIdx], false);
            renderer.endCommandBuffer(imGuiCommandBuffers[frameIdx]);

            bool wasWindowResized = false;
            renderer.endFrame(std::vector<VkCommandBuffer>({commandBuffer, imGuiCommandBuffers[frameIdx]}), wasWindowResized);

            if (wasWindowResized) {
                for (auto framebuffer : imGuiFramebuffers) {
                    vkDestroyFramebuffer(device.device(), framebuffer, nullptr);
                }

                renderer.getSwapChain()->createFramebuffers(std::vector({renderer.getSwapChain()->getImageViews()}),
                                                            imGuiFramebuffers,
                                                            imGuiRenderPass);
            }
        }
    }

    vkDeviceWaitIdle(device.device());
}

void Application::loadEntities() {
    std::shared_ptr<Texture> texture = Texture::createTextureFromFile(device, (root_path + "/textures/test.jpg"));
    std::shared_ptr<Material> material = std::make_shared<Material>(texture);

    std::shared_ptr<Mesh> mesh =
        Mesh::createModelFromFile(device, (root_path + "/models/flat_vase.obj").c_str());
    auto flatVase = Entity::createEntity();
    flatVase.mesh = mesh;
    flatVase.material = material;
    flatVase.transform.translation = {-.5f, .5f, 2.5f};
    flatVase.transform.scale = {3.f, 1.5f, 3.f};
    entities.push_back(std::move(flatVase));

    mesh = Mesh::createModelFromFile(device, (root_path + "/models/smooth_vase.obj").c_str());
    auto smoothVase = Entity::createEntity();
    smoothVase.mesh = mesh;
    smoothVase.material = material;
    smoothVase.transform.translation = {.5f, .5f, 2.5f};
    smoothVase.transform.scale = {3.f, 1.5f, 3.f};
    entities.push_back(std::move(smoothVase));
}

static void checkVkResult(VkResult err) {
    if (err == VK_SUCCESS)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

void Application::setupImGuiContext() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(window.getGLFWwindow(), true);
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = device.instance();
    initInfo.PhysicalDevice = device.physicalDevice();
    initInfo.Device = device.device();
    initInfo.QueueFamily = device.queueFamilyIndices().graphicsFamily;
    initInfo.Queue = device.graphicsQueue();
    initInfo.PipelineCache = VK_NULL_HANDLE;

    createImGuiDescriptorPool();
    initInfo.DescriptorPool = imGuiDescriptorPool;

    initInfo.Allocator = VK_NULL_HANDLE;
    initInfo.MinImageCount = device.getSwapChainSupport().capabilities.minImageCount + 1;
    initInfo.ImageCount = renderer.getSwapChainImageCount();
    initInfo.CheckVkResultFn = checkVkResult;

    createImGuiRenderPass();
    ImGui_ImplVulkan_Init(&initInfo, imGuiRenderPass);

    device.createCommandPool(imGuiCommandPool, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    renderer.createCommandBuffers(imGuiCommandBuffers, imGuiCommandPool);

    renderer.getSwapChain()->createFramebuffers(std::vector({renderer.getSwapChain()->getImageViews()}),
                                                imGuiFramebuffers,
                                                imGuiRenderPass);

    VkCommandBuffer commandBuffer = device.beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    device.endSingleTimeCommands(commandBuffer);
}

void Application::createImGuiDescriptorPool() {
    VkDescriptorPoolSize poolSizes[] =
        {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
    poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
    poolInfo.pPoolSizes = poolSizes;

    if (vkCreateDescriptorPool(device.device(), &poolInfo, nullptr, &imGuiDescriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create separate ImGui descriptor pool");
    }
}

void Application::createImGuiRenderPass() {
    VkAttachmentDescription attachment = {};
    attachment.format = renderer.getSwapChain()->getSwapChainImageFormat();
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachment = {};
    colorAttachment.attachment = 0;
    colorAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachment;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments = &attachment;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 1;
    info.pDependencies = &dependency;
    if (vkCreateRenderPass(device.device(), &info, nullptr, &imGuiRenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create ImGui's separate render pass");
    }
}

}  // namespace vkr
