/*
*   Modified version of the framework provided by Brendan Galea in his Vulkan
*   tutorial series (https://github.com/blurrypiano/littleVulkanEngine) 
*   Copyright (c) 2020 Brendan Galea
*/

#include <InputController.hpp>

// std
#include <limits>

namespace vkr {

void InputController::moveInPlaneXZ(
    GLFWwindow* window, float dt, Entity& entity) {
    glm::vec3 rotate{0};
    if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y -= 1.f;
    if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y += 1.f;
    if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x -= 1.f;
    if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x += 1.f;

    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
        entity.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
    }

    // limit pitch values between about +/- 85ish degrees
    entity.transform.rotation.x = glm::clamp(entity.transform.rotation.x, -1.5f, 1.5f);
    entity.transform.rotation.y = glm::mod(entity.transform.rotation.y, glm::two_pi<float>());

    float yaw = entity.transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    glm::vec3 moveDir{0.f};
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir -= rightDir;
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir += rightDir;
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir -= upDir;
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir += upDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        entity.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
    }
}
}  // namespace vkr