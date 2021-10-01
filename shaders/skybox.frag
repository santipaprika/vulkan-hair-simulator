#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 normalWS;
layout (location = 2) in vec2 fragTexCoord;

layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform UniformBufferObject {
    mat4 projectionView;
    mat4 model;
    mat4 normalMatrix;
    vec3 camPos;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

void main() {
    outColor = texture(texSampler,fragTexCoord*3);
}
