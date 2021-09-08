#version 450

layout (location = 0) in vec3 fragColor;

layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform UniformBufferObject {
    mat4 transform;
    mat4 normalMatrix;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

layout(push_constant) uniform Push {
    float brightness;
} push;

void main() {
    outColor = vec4(0.4);
}
