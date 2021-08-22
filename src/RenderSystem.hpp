#pragma once

#include "Camera.hpp"
#include "Device.hpp"
#include "Entity.hpp"
#include "Pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace vkr {
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

  Device &Device;

  std::unique_ptr<Pipeline> Pipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace vkr
