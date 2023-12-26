#version 430 core
out vec4 FragColor;

in vec4 vertex_color;
in vec2 fragment_texCoord;

uniform sampler2D texture_default;
uniform vec4 key_color;
void main()
{
    vec4 tex_col = texture(texture_default, fragment_texCoord);
    if(key_color == tex_col)
        discard;
        
    FragColor = vec4(tex_col);
}