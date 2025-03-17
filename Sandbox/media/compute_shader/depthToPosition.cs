#version 450 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, r16f) uniform image2D depthTexture;

uniform float lowerBound;
uniform float upperBound;

void main()
{
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);

    float depthValue = imageLoad(depthTexture, pixelCoords).r;
    depthValue = clamp(depthValue, lowerBound, upperBound);

    imageStore(depthTexture, pixelCoords, vec4(depthValue, 0.0, 0.0, 0.0));
}
#pragma once
