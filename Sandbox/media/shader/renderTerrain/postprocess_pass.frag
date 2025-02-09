#version 430 core

in vec2 vTexCoord; 
out vec4 fColor;

uniform sampler2D inputTexture;

void main()
{
    vec4 texColor = texture(inputTexture, vec2(vTexCoord.x, 1.0 - vTexCoord.y));
    fColor = texColor;
}
