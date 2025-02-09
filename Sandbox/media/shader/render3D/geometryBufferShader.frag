#version 450 core

layout (location = 0) out vec4 fPosition; 
layout (location = 1) out vec4 fNormal;
layout (location = 2) out vec4 fColor;
layout (location = 3) out vec4 fSpecular;
layout (location = 5) out vec4 fLocalNormal;

in vec4 vColor;
in vec4 vPosition;
in vec4 vNormal;
in vec3 vTangent;
in vec3 vBitangent;
in vec4 vLocalNormal;
in vec2 vTexCoord;

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

void main()
{
    vec3 texAlbedo   = texture(material.albedoTexture, vTexCoord).rgb;
    vec3 texSpecular = texture(material.specularTexture, vTexCoord).rgb;
    vec3 texNormal   = texture(material.normalTexture, vTexCoord).rgb;

    mat3 tbn = mat3(normalize(vTangent), normalize(vBitangent), normalize(vNormal.xyz));
    vec3 normalFromTexture = texNormal * 2.0 - vec3(1.0);
    fNormal = vec4(normalize(tbn * normalFromTexture), 0.0);

    fPosition = vPosition;
    fColor = vec4(material.kd * texAlbedo, 1.0);
    fSpecular = vec4(material.ks * texSpecular, material.kn / 300.0);
    fLocalNormal = vLocalNormal;
}
