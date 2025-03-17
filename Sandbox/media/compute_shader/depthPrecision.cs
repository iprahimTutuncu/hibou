#version 450 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, r16) uniform image2D depthTexture;

uniform float depthPrecision;

void main()
{
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);

    float depthValue = imageLoad(depthTexture, pixelCoords).r;
    depthValue = round(depthValue / depthPrecision) * depthPrecision;

    imageStore(depthTexture, pixelCoords, vec4(depthValue, 0.0, 0.0, 0.0));
}
