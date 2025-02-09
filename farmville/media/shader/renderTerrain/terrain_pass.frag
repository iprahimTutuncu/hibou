#version 430 core

// Output variables
layout (location = 0) out vec4 positionDepth; 
layout (location = 1) out vec4 normal;
layout (location = 2) out vec4 albedo;
layout (location = 3) out vec4 specular;
//layout (location = 4) out vec4 emissive;

// Input variables
in float vDepth;
in vec2 vTexCoord;
in vec3 vPosition;
in vec4 vColor;

// Uniform samplers for textures
uniform sampler2D spritesTextureAlbedo;
uniform sampler2D spritesTextureDepth;
uniform sampler2D spritesTexturePosition;

uniform sampler2D terrainTextureDepth;
uniform sampler2D terrainTextureAlbedo;
uniform sampler2D terrainTexturePosition;
uniform sampler2D terrainTextureNormal;

void main()
{
    // Flip the y-coordinate for sprite texture coordinates
    vec2 flippedTexCoord = vec2(vTexCoord.x, 1.0 - vTexCoord.y);

    float spriteDepth = texture(spritesTextureDepth, flippedTexCoord).x;
    float terrainDepth = texture(terrainTextureDepth, vTexCoord).x;
    vec3 spritePosition = texture(spritesTexturePosition, flippedTexCoord).xyz;
    

    // Determine which position and albedo to use based on depth comparison
    if (spriteDepth < terrainDepth)
    {
        positionDepth = vec4(spritePosition, spriteDepth); //currently no position available for sprite, ideally put atleast the floor position
        albedo = texture(spritesTextureAlbedo, flippedTexCoord);
        normal = vec4(0.0, 0.0, 0.0, 0.0);
    }
    else
    {
        positionDepth = texture(terrainTexturePosition, vTexCoord);
        positionDepth.a = terrainDepth;
        albedo = texture(terrainTextureAlbedo, vTexCoord);
        normal = texture(terrainTextureNormal, vTexCoord);
    }
}
