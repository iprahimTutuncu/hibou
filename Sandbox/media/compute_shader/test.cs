#version 430

// Define the local workgroup size
layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

// Define the layout for the SSBO containing vertex data
layout(std430, binding = 0) buffer VertexBuffer
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    vec3 tangent;
    vec3 bitangent;
    vec4 color;
}
vertices[];

uniform uint vertexCount;

// Texture sampler and output image
layout(binding = 0, rgba32f) uniform image2D OutputImage;
layout(binding = 1) uniform sampler2D SpriteTexture;

void main()
{
    // Get the global ID of the current thread
    uint globalID = gl_GlobalInvocationID.x;

    // Sample from the sprite texture using texCoord
    vec4 texColor = texture(SpriteTexture, vertices[globalID].texCoord);

    // Modify the vertex properties here if needed
    vertices[globalID].color *= texColor;

    // Write to the output image
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    imageStore(OutputImage, pixelCoords, texColor);
}
