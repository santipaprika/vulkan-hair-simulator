#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 normalWS;

layout(push_constant) uniform Push {
    mat4 transform; // projection * view * model
    mat4 normalMatrix;
} push;

void main() {
    gl_Position = push.transform * vec4(position, 1.0);

    normalWS = normalize(mat3(push.normalMatrix) * normal);
    fragColor = color;
}
