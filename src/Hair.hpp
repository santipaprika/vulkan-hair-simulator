#pragma once

#include <cyHairFile.h>
#include <vulkan/vulkan.h>

#include <glm/glm.hpp>
#include <vector>

namespace vkr {

class Hair {
   public:
    struct Vertex {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 normal{};
        // glm::vec2 uv{};

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex &other) const {
            return position == other.position && color == other.color && normal == other.normal;
        }
    };

    Hair(const char *filename);
    ~Hair();

    void loadHairModel(const char *filename, cyHairFile &hairfile, float *&dirs);
    // void drawHairModel(const cyHairFile &hairfile, float *dirs);

   private:
    cyHairFile hair;
    float *dirs;
};

}  // namespace vkr
