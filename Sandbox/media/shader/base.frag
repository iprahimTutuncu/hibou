#version 430 core
out vec4 FragColor;

uniform mat4 VP;
uniform mat4 view;

in vec4 vertex_color;
in vec2 fragment_texCoord;
uniform sampler2D diffuse_texture;

void main()
{
    vec3 tex_col = texture(diffuse_texture, fragment_texCoord).rgb;
    FragColor = vec4(tex_col, 1.0) * vertex_color;
}