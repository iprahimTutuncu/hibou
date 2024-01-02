#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int meshId;

out vec4 vColor;
out vec4 vPosition;
out vec4 vNormal;
out vec3 vTangent;
out vec3 vBitangent;
out vec4 vLocalNormal;
out vec2 vTexCoord;

void main()
{
    vTexCoord = texCoord;
    vec4 worldPos = model * vec4(position.xyz, 1.0);

    gl_Position = projection * view * worldPos;

    vColor = color;
    vPosition = worldPos;
    vTangent = tangent;
    vBitangent = bitangent;
    vNormal = (transpose(inverse(model)) * vec4(normal, 0.0));

    vLocalNormal = vec4(normal, 1.0);
}
