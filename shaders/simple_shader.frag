#version 450

layout (location = 0) in vec3 fragColor;

layout (location = 0) out vec4 outColor;


const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));
const float AMBIENT = 0.02;

layout(push_constant) uniform Push {
  mat4 transform; // projection * view * model
  mat4 normalMatrix;
} push;

void main() {
  outColor = vec4(fragColor, 1.0);
}
