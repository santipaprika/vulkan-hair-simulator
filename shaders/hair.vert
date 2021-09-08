#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 direction;

layout(location = 0) out vec3 fragColor;

layout (binding = 0) uniform UniformBufferObject {
    mat4 transform;
    mat4 normalMatrix;
} ubo;

void main() {
    gl_Position = ubo.transform * vec4(position, 1.0);

    // directionWS = normalize(mat3(ubo.normalMatrix) * direction);
    fragColor = color;
}
