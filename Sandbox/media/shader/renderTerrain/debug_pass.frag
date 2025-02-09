#version 430 core

in vec2 vTexCoord; 
out vec4 fColor;

uniform sampler2D inputTexture;
uniform int isDepth;
uniform int isNormal;
uniform int isPosition;
uniform int isAlbedo;

void main()
{
    vec4 texColor;

    if(isAlbedo == 1)
        texColor = texture(inputTexture, vec2(vTexCoord.x, 1.0 - vTexCoord.y));
    else if(isDepth == 1){
        texColor.rgb = texture(inputTexture, vec2(vTexCoord.x, 1.0 - vTexCoord.y)).aaa;
        texColor.a = 1.0;
    }else if(isPosition == 1 || isNormal == 1){
        texColor.rgb = texture(inputTexture, vec2(vTexCoord.x, 1.0 - vTexCoord.y)).rgb;
        texColor.a = 1.0;
    }
    fColor = texColor;
}
