#version 430

// Define the layout of the output image
layout(binding = 0, rgba8) uniform writeonly image2D destTex;

// Uniforms
uniform vec3 position;
uniform vec2 normalBlurr;
uniform float screenWidth;
uniform float screenHeight;
uniform int terrainSize;
uniform int normalBlurIteration;
uniform float LOD;
uniform float LODStartAt;
uniform float angle;
uniform float horizon;
uniform float scaleHeight;
uniform float spriteRenderDistance;
uniform float terrainRenderDistance;
uniform float shininess;
uniform int FOV;
uniform int blurType;

// Define the work group size
layout(local_size_x = 16, local_size_y = 16) in;

void main()
{
    // Get the global ID of the thread
    ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);

    // Define the color (for example, green)
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0); // RGBA

    // Write the color to the texture
    for (int i = 0; i < 1024; i++) {
        imageStore(destTex, storePos + ivec2(i, 0), color);
    }
}
