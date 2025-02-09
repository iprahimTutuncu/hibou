#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec4 color;

uniform mat4 model;
uniform mat4 lightView;
uniform mat4 lightProjection;

void main()
{
    gl_Position = lightProjection * lightView * model * vec4(position.xyz, 1.0);
}
