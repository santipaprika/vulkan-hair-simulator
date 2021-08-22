#include "Application.hpp"

#include "InputController.hpp"
#include "Camera.hpp"
#include "RenderSysten.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
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
  RenderSystem RenderSystem{Device, Renderer.getSwapChainRenderPass()};
  Camera camera{};

  auto viewerObject = Entity::createEntity();
  InputController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();
  while (!Window.shouldClose()) {
    glfwPollEvents();

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    cameraController.moveInPlaneXZ(Window.getGLFWwindow(), frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

    float aspect = Renderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

    if (auto commandBuffer = Renderer.beginFrame()) {
      Renderer.beginSwapChainRenderPass(commandBuffer);

      RenderSystem.renderEntities(commandBuffer, entities, camera);

      Renderer.endSwapChainRenderPass(commandBuffer);
      Renderer.endFrame();
    }
  }

  vkDeviceWaitIdle(Device.device());
}

void Application::loadEntities() {
  std::shared_ptr<Mesh> Mesh =
      Mesh::createModelFromFile(Device, "models/flat_vase.obj");
  auto flatVase = Entity::createEntity();
  flatVase.model = Mesh;
  flatVase.transform.translation = {-.5f, .5f, 2.5f};
  flatVase.transform.scale = {3.f, 1.5f, 3.f};
  entities.push_back(std::move(flatVase));

  Mesh = Mesh::createModelFromFile(Device, "models/smooth_vase.obj");
  auto smoothVase = Entity::createEntity();
  smoothVase.model = Mesh;
  smoothVase.transform.translation = {.5f, .5f, 2.5f};
  smoothVase.transform.scale = {3.f, 1.5f, 3.f};
  entities.push_back(std::move(smoothVase));
}

}  // namespace vkr
