#version 450 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, r16) uniform readonly image2D srcR16Texture;
layout(binding = 1, r16) uniform writeonly image2D destR16Texture;

uniform vec4 occlusionColor;

void main()
{
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);

    float depthValue = imageLoad(srcR16Texture, pixelCoords).r;
    imageStore(destR16Texture, pixelCoords, vec4(depthValue, 0, 0, 0));
}
