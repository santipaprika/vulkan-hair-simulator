/*
*   Modified version of the framework provided by Brendan Galea in his Vulkan
*   tutorial series (https://github.com/blurrypiano/littleVulkanEngine)
*   Copyright (c) 2020 Brendan Galea
*/

#include <Application.hpp>

#include <Camera.hpp>
#include <ImGuiHelper.hpp>
#include <InputController.hpp>
#include <RenderSystem.hpp>
#include <Hair.hpp>

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
}

void Application::run() {
    RenderSystem renderSystem{device, renderer.getSwapChainRenderPass(), entities};
    ImGuiHelper imGuiHelper(*this);
    Camera camera{};

    auto viewerObject = Entity::createEntity();
    InputController cameraController{};

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

            VkCommandBuffer imGuiCommandBuffer = imGuiHelper.renderImGui();

            bool wasWindowResized = false;
            renderer.endFrame(std::vector<VkCommandBuffer>({commandBuffer, imGuiCommandBuffer}), wasWindowResized);

            if (wasWindowResized) {
                imGuiHelper.updateImGuiFramebuffers();
            }
        }
    }

    vkDeviceWaitIdle(device.device());
}

void Application::loadEntities() {
    std::string textures_path(TEXTURES_PATH); 
    std::shared_ptr<Texture> texture = Texture::createTextureFromFile(device, (textures_path + "/test.jpg"));
    std::shared_ptr<Material> material = std::make_shared<Material>(texture);

    std::string models_path(MODELS_PATH); 
    std::shared_ptr<Mesh> mesh =
        Mesh::createModelFromFile(device, (models_path + "/flat_vase.obj").c_str());
    auto flatVase = Entity::createEntity();
    flatVase.mesh = mesh;
    flatVase.material = material;
    flatVase.transform.translation = {-.5f, .5f, 2.5f};
    flatVase.transform.scale = {3.f, 1.5f, 3.f};
    entities.push_back(std::move(flatVase));

    mesh = Mesh::createModelFromFile(device, (models_path + "/smooth_vase.obj").c_str());
    auto smoothVase = Entity::createEntity();
    smoothVase.mesh = mesh;
    smoothVase.material = material;
    smoothVase.transform.translation = {.5f, .5f, 2.5f};
    smoothVase.transform.scale = {3.f, 1.5f, 3.f};
    entities.push_back(std::move(smoothVase));

    Hair hair( (models_path + "/wWavy.hair").c_str() );
}

}  // namespace vkr
