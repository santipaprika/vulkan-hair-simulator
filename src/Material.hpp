#pragma once

#include "Texture.hpp"

// libs
#include <glm/glm.hpp>
#include <memory>

namespace vkr {

class Material {
   public:
    Material(std::shared_ptr<Texture> albedo, glm::vec4 diffuseColor = glm::vec4(1.f,1.f,1.f,1.f));
    ~Material();

    std::shared_ptr<Texture> getAlbedo() { return (_albedo); }

   private:
    std::shared_ptr<Texture> _albedo;
    glm::vec4 _diffuseColor;
};

}  // namespace vkr