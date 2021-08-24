#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 normalWS;
layout (location = 2) in vec2 fragTexCoord;

layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform UniformBufferObject {
    vec4 color;
} ubo;
layout(binding = 1) uniform sampler2D texSampler;


const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));
vec3 AMBIENT = ubo.color.xyz;

layout(push_constant) uniform Push {
    mat4 transform; // projection * view * model
    mat4 normalMatrix;
} push;

void main() {
    vec3 lightIntensity = AMBIENT + vec3(max(dot(normalWS, DIRECTION_TO_LIGHT), 0));
    outColor = texture(texSampler,fragTexCoord*3) * vec4(fragColor * lightIntensity, 1.0);
}
