#version 460

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, r16) uniform image2D r16Texture;  // Corrected format
layout(binding = 1, rgba8) writeonly uniform image2D rgbaTexture;

void main()
{
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);

    // Read the normalized floating-point value from the texture
    float grayscaleValue = imageLoad(r16Texture, pixelCoords).r;

    // Store the grayscale value as an RGBA color
    imageStore(rgbaTexture, pixelCoords, vec4(grayscaleValue, grayscaleValue, grayscaleValue, 1.0));
}
