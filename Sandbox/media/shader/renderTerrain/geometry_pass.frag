#version 430 core

layout (location = 0) out vec4 depth; 
layout (location = 1) out vec4 normal;
layout (location = 2) out vec4 albedo;
layout (location = 3) out vec4 specular;
//layout (location = 4) out vec4 emissive;

in float vDepth;
in vec2 vTexCoord;
in vec3 vPosition;
in vec3 vExternalPosition;
in vec4 vColor;

uniform sampler2D textureDefault;

uniform vec4 keyColor;
uniform float maximumDepth;
uniform float scaleHeight;

/*
struct Material {
    vec3 ka;
    vec3 ke;
    vec3 kd;
    vec3 ks;
    float kn;

    sampler2D albedoTexture;
    sampler2D specularTexture;
    sampler2D normalTexture;
}; 

uniform Material material;
*/

void main()
{
    vec4 texCol = texture(textureDefault, vTexCoord);
    if(keyColor == texCol)
        discard;

    gl_FragDepth = vDepth/maximumDepth;
    albedo = vec4(texCol.xyz, 1.0);
    depth = vec4(vDepth / maximumDepth, 0.0, 0.0, 1.0);
    //position = vec4(vPosition, 1.0);
}