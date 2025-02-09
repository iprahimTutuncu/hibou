#version 430 core

in vec2 vTexCoord; 
in vec4 vColor;
out vec4 fColor;

uniform vec4 keyColor;
uniform sampler2D textureDefault;

void main()
{
    vec4 texCol = texture(textureDefault, vTexCoord);
    if(keyColor == texCol)
        discard;

    fColor = texCol * vColor;
}
