#pragma once

#include <Camera.hpp>
#include <Entity.hpp>
#include <Texture.hpp>

namespace vkr {

class Scene {
   public:
    Scene(Device& device);
    ~Scene();

    // Initialization
    void initialize();
    void loadEntities();
    void loadLights();
    void loadCameraSkybox();

    // Getters
    std::vector<Entity>& getEntities() { return entities; }
    std::vector<Entity>& getLightEntities() { return lights; }
    Camera& getMainCamera() { return mainCamera; }

    // Update
    void updateScene();

   private:
    std::vector<Entity> entities;
    std::vector<Entity> lights;
    std::vector<Texture> textures;
    Camera mainCamera;

    Device& device;
};

}  // namespace vkr
