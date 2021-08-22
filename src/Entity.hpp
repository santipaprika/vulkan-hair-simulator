#pragma once

#include "Mesh.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace vkr {

struct TransformComponent {
  glm::vec3 translation{};
  glm::vec3 scale{1.f, 1.f, 1.f};
  glm::vec3 rotation{};

  // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
  // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
  // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
  glm::mat4 mat4();

  glm::mat3 normalMatrix();
};

class Entity {
 public:
  using id_t = unsigned int;

  static Entity createEntity() {
    static id_t currentId = 0;
    return Entity{currentId++};
  }

  Entity(const Entity &) = delete;
  Entity &operator=(const Entity &) = delete;
  Entity(Entity &&) = default;
  Entity &operator=(Entity &&) = default;

  id_t getId() { return id; }

  std::shared_ptr<Mesh> model{};
  glm::vec3 color{};
  TransformComponent transform{};

 private:
  Entity(id_t objId) : id{objId} {}

  id_t id;
};
}  // namespace vkr
