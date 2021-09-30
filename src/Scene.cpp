#include <Scene.hpp>
#include <Utils.hpp>

namespace vkr {

Scene::Scene(Device& device) : device{device} {
}

Scene::~Scene() {
}

void Scene::initialize() {
    loadEntities();
    loadLights();
    loadCameraSkybox();
}

void Scene::loadEntities() {
    std::string textures_path(TEXTURES_PATH);
    std::shared_ptr<Texture> texture = Texture::createTextureFromFile(device, (textures_path + "/test.jpg"));
    std::shared_ptr<Material> material = std::make_shared<Material>(texture);

    std::string models_path(MODELS_PATH);

    // Mesh Entities
    std::shared_ptr<Mesh> mesh =
        Mesh::createModelFromFile(device, (models_path + "/flat_vase.obj").c_str());
    auto flatVase = Entity::createEntity();
    flatVase.mesh = mesh;
    flatVase.material = material;
    flatVase.transform.translation = {-.5f, .5f, 2.5f};
    flatVase.transform.scale = {1.5f, 1.5f, 1.5f};
    entities.push_back(std::move(flatVase));

    mesh = Mesh::createModelFromFile(device, (models_path + "/smooth_vase.obj").c_str());
    auto smoothVase = Entity::createEntity();
    smoothVase.mesh = mesh;
    smoothVase.material = material;
    smoothVase.transform.translation = {.5f, .5f, 2.5f};
    smoothVase.transform.scale = {1.5f, 1.5f, 1.5f};
    entities.push_back(std::move(smoothVase));

    // Hair Entities
    auto hairEntity = Entity::createEntity();
    hairEntity.hair = std::make_shared<Hair>(device, (models_path + "/wWavy.hair").c_str());

    // TO DO: Might not have a material, support multiple descriptor set layouts!
    hairEntity.material = material;
    hairEntity.transform.translation = {0.f, 2.f, 2.5f};
    hairEntity.transform.scale = {0.03f, 0.03f, 0.03f};
    hairEntity.transform.rotation = {PI_2, PI_2, 0};

    entities.push_back(std::move(hairEntity));
}

void Scene::loadLights() {
    // Light Entities
    auto mainLight = Entity::createEntity();
    Light light{1.f, glm::vec3{1.f, 1.f, 1.f}};
    mainLight.light = std::make_shared<Light>(1.f, glm::vec3(1.f, 1.f, 1.f));
    mainLight.transform.translation = {0.f, 2.f, 0.f};
    mainLight.transform.rotation = {PI_2, PI_2, 0};

    lights.push_back(std::move(mainLight));
}

void Scene::loadCameraSkybox() {
    mainCamera.loadSkybox(device);
}

void Scene::updateScene() {
}

}  // namespace vkr