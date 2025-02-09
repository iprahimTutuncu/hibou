#version 430 core

layout (location = 0) out vec4 depth; 
layout (location = 1) out vec4 normal;
layout (location = 2) out vec4 albedo;
layout (location = 3) out vec4 specular;
layout (location = 4) out vec4 position;
//layout (location = 4) out vec4 emissive;

in float vDepth;
in vec2 vTexCoord;
in vec3 vPosition;
in vec4 vColor;

uniform sampler2D textureDefault;
uniform vec3 directionFromCamera;

uniform vec4 keyColor;
uniform float maximumDepth;
uniform float scaleHeight;
uniform float frameHeight;

uniform vec2 extScale;
uniform vec3 extPosition;
uniform float cameraAngle;

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
    float sinphi = sin(-cameraAngle);
    float cosphi = cos(-cameraAngle);
    vec4 texCol = texture(textureDefault, vTexCoord);
    if(keyColor == texCol)
        discard;

    gl_FragDepth = vDepth/maximumDepth;
    albedo = vec4(texCol.xyz, 1.0);
    depth = vec4(vDepth / maximumDepth, 0.0, 0.0, 1.0);

    mat3 scalingMatrix = mat3(
        extScale.x, 0.0f, 0.0f,
        0.0f, extScale.y, 0.0f,
        0.0f, 0.0f, 1.0f 
    );


    mat3 rotationMatrix = mat3(
        cosphi, 0.0, sinphi,   
        0.0, 1.0, 0.0,         
        -sinphi, 0.0, cosphi   
    );

    vec3 offset = vec3(0.0, frameHeight / 2.0, 0.0);
    vec3 origin = vPosition - offset;

    // TODO dans l'avion
    // vérifier les Y si il scale adéquatement et tout, si 32 px
    // possible que something est off quand on retourne le height
    // par la suite, je vois pas en quoi le height du terrain co-ordonne avec
    // le height du sprite, y'a de l'étude à faire
    //x et z work good

    vec3 scaledOrigin = scalingMatrix * origin;
    vec3 rotateScaledOrigin = rotationMatrix * scaledOrigin; 
    vec3 final = rotateScaledOrigin + extPosition + offset;

    position = vec4(final, 1.0);
}