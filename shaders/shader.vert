#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 uv;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(position, 1.0);
    fragColor = vec3(uv, 0.0);
}
