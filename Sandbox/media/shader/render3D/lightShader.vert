#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 camera_position;
uniform vec3 camera_front;

out vec4 vColor;
out vec2 vTexCoord;
out vec3 vNormal;

void main()
{
    gl_Position = vec4(position.xy, -position.z, 1);
    vColor = color;
    vTexCoord = texCoord;

    //Fundamental of computer graphics, 4th edition, page 127 pour le transformation sur la normal
    vNormal = (transpose(inverse(model)) * vec4(normal, 1.0)).xyz;
}