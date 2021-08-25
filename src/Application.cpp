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
#include <stdexcept>

namespace vkr {

Application::Application() { loadEntities(); }

Application::~Application() {}

void Application::run() {
    RenderSystem renderSystem{device, renderer.getSwapChainRenderPass(), entities[0].material->getAlbedo()};
    Camera camera{};

    auto viewerObject = Entity::createEntity();
    InputController cameraController{};

    // setupImGuiContext();

    auto currentTime = std::chrono::high_resolution_clock::now();
    while (!window.shouldClose()) {
        glfwPollEvents();

        // ImGui_ImplVulkan_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();
        // ImGui::ShowDemoWindow();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = renderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

        // Rendering
        // ImGui::Render();
        // ImDrawData* draw_data = ImGui::GetDrawData();

        if (auto commandBuffer = renderer.beginFrame()) {
            renderer.beginSwapChainRenderPass(commandBuffer);

            renderSystem.renderEntities(commandBuffer, entities, camera);

            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }

    vkDeviceWaitIdle(device.device());

    // ImGui_ImplVulkan_Shutdown();
    // ImGui_ImplGlfw_Shutdown();
    // ImGui::DestroyContext();
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
    flatVase.material = material;
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
    // initInfo.DescriptorPool = device.getDescriptorPool();
    initInfo.Allocator = VK_NULL_HANDLE;
    initInfo.MinImageCount = device.getSwapChainSupport().capabilities.minImageCount + 1;
    initInfo.ImageCount = renderer.getSwapChainImageCount();
    initInfo.CheckVkResultFn = checkVkResult;
    ImGui_ImplVulkan_Init(&initInfo, renderer.getSwapChainRenderPass());

    // Upload Fonts
    {
        // if (renderer.acquireNextSwapChainImage()) {
        // Use any command queue
        VkCommandPool commandPool = device.getCommandPool();
        VkCommandBuffer commandBuffer = renderer.getCurrentCommandBuffer(false);

        if (vkResetCommandPool(device.device(), commandPool, 0) != VK_SUCCESS) {
            throw std::runtime_error("failed to reset command pool!");
        }

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin command buffer for loading ImGui font!");
        }

        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

        VkSubmitInfo endInfo = {};
        endInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        endInfo.commandBufferCount = 1;
        endInfo.pCommandBuffers = &commandBuffer;
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to end command buffer for loading ImGui font!");
        }

        if (vkQueueSubmit(device.graphicsQueue(), 1, &endInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit queue for loading ImGui font!");
        }

        if (vkDeviceWaitIdle(device.device()) != VK_SUCCESS) {
            throw std::runtime_error("failed to wait for idle device when loading ImGui font!");
        }

        ImGui_ImplVulkan_DestroyFontUploadObjects();
        // }
    }
}

}  // namespace vkr
