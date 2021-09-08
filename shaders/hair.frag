#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 directionWS;

layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform UniformBufferObject {
    mat4 transform;
    mat4 normalMatrix;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

layout(push_constant) uniform Push {
    float brightness;
} push;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));
vec3 AMBIENT = vec3(0.1);

void main() {
    vec3 lightIntensity = AMBIENT + abs(dot(directionWS, DIRECTION_TO_LIGHT));
    outColor = vec4(fragColor * lightIntensity, 1.0) + vec4(vec3(push.brightness), 0.0);
}
