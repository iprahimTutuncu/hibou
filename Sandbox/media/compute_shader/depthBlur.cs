#version 450 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, r16) uniform image2D depthTexture;
layout(binding = 1, r16) uniform image2D depthBlurredTexture;
layout(binding = 2, r16) uniform readonly image2D depthTextureUnmodified;

uniform ivec2 iDirection;
uniform int kernelSize;
uniform float blurableLimitDistance;

#define MAX_KERNEL_SIZE 129

uniform float coeffs[MAX_KERNEL_SIZE] ;

void main()
{
    ivec2 size = imageSize(depthTexture);
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);

    if (pixelCoords.x < size.x && pixelCoords.y < size.y)
    {
        float sum = 0.0;
        int M = kernelSize / 2;
        float previous = imageLoad(depthTexture, pixelCoords).r;

        for (int i = 0; i < kernelSize; ++i)
        {
            int offset = i - M;
            float weight = coeffs[i];
            ivec2 neighborCoords = pixelCoords + iDirection * offset;
            neighborCoords = clamp(neighborCoords, ivec2(0), size - ivec2(1));
            float depth = imageLoad(depthTexture, neighborCoords).r;
            float unmodifiedDepth = imageLoad(depthTextureUnmodified, pixelCoords).r;

            // Compare with the unmodified depth to determine if it should contribute to the blur
            if (abs(depth - unmodifiedDepth) < blurableLimitDistance)
            {
                sum += weight * depth;
                previous = unmodifiedDepth;
            }
            else
            {
                sum += weight * previous; // Use the current pixel's depth if depth difference is too large
            }
        }

        // Store the final blurred value into the output image
        imageStore(depthBlurredTexture, pixelCoords, vec4(sum, 0.0, 0.0, 0.0));
    }
}
