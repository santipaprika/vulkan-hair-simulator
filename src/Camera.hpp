/*
*   Modified version of the framework provided by Brendan Galea in his Vulkan
*   tutorial series (https://github.com/blurrypiano/littleVulkanEngine) 
*   Copyright (c) 2020 Brendan Galea
*/

#pragma once

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace vkr {

class Camera {
   public:
    void setOrthographicProjection(
        float left, float right, float top, float bottom, float near, float far);
    void setPerspectiveProjection(float fovy, float aspect, float near, float far);

    void setViewDirection(
        glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
    void setViewTarget(
        glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
    void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

    const glm::mat4& getProjection() const { return projectionMatrix; }
    const glm::mat4& getView() const { return viewMatrix; }
    glm::vec3 getPosition() { return glm::vec3{viewMatrix[3][0], viewMatrix[3][1], viewMatrix[3][2]}; }

   private:
    glm::mat4 projectionMatrix{1.f};
    glm::mat4 viewMatrix{1.f};
};
}  // namespace vkr