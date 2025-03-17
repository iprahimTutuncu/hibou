#version 450 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, rgba8) uniform readonly image2D maskTexture;
layout(binding = 1, rgba8) uniform readonly image2D RGBA8Texture;
layout(binding = 2, rgba8) uniform writeonly image2D modifiedRGBA8Texture;

uniform vec4 occlusionColor;

void main()
{
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);

    vec4 maskColor = imageLoad(maskTexture, pixelCoords);
    vec4 colorValue = imageLoad(RGBA8Texture, pixelCoords);
    bool isTarget = all(equal(maskColor.rgb, occlusionColor.rgb));

    vec4 newColor = isTarget ? vec4(0.0) : colorValue;

    imageStore(modifiedRGBA8Texture, pixelCoords, newColor);
}
