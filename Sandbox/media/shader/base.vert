#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec4 color;

uniform mat4 VP;
uniform mat4 view;
uniform vec3 camera_position;
uniform vec3 camera_front;
uniform vec3 translation;

out vec4 vertex_color;
out vec2 fragment_texCoord;

void main()
{
    gl_Position = VP * vec4(position + translation, 1.0);
    vertex_color = color;
    fragment_texCoord = texCoord;
}