#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 normalWS;

layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform UniformBufferObject {
    vec4 color;
} ubo;


const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));
const float AMBIENT = 0.02;

layout(push_constant) uniform Push {
    mat4 transform; // projection * view * model
    mat4 normalMatrix;
} push;

void main() {
    float a = ubo.color.x;
    float lightIntensity = AMBIENT + max(dot(normalWS, DIRECTION_TO_LIGHT), 0);
    outColor = vec4(fragColor * lightIntensity, 1.0) * ubo.color;
}
