#version 450 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, rgba8) uniform readonly image2D maskTexture;
layout(binding = 1, r16) uniform readonly image2D r16Texture;
layout(binding = 2, r16) uniform writeonly image2D modifiedR16Texture;

uniform vec4 occlusionColor;

void main()
{
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);

    vec4 maskColor = imageLoad(maskTexture, pixelCoords);
    float depthValue = imageLoad(r16Texture, pixelCoords).r;
    bool isTarget = all(equal(maskColor.rgb, occlusionColor.rgb));

    float newDepth = isTarget ? 0.0 : depthValue;

    imageStore(modifiedR16Texture, pixelCoords, vec4(newDepth, 0, 0, 0));
}
