#pragma once

#include "Device.hpp"
#include "Entity.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

// std
#include <memory>
#include <vector>

namespace vkr {
class Application {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  Application();
  ~Application();

  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;

  void run();

 private:
  void loadEntities();

  Window Window{WIDTH, HEIGHT, "Vulkan Tutorial"};
  Device Device{Window};
  Renderer Renderer{Window, Device};

  std::vector<Entity> entities;
};
}  // namespace vkr
