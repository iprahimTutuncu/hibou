#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec4 color;

out vec4 vColor;
out vec2 vTexCoord;

void main() {
    gl_Position = vec4(position.xy, -position.z, 1);
    vTexCoord = texCoord;
    vColor = color;
}