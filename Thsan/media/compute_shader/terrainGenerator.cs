#version 430

// Define the local workgroup size
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

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

void main()
{
    // Get the global ID of the current thread
    uint globalID = gl_GlobalInvocationID.x;

    // Modify the vertex properties here if needed
    vertices[globalID].color.r *= 0.0;
    vertices[globalID].color.g *= 0.0;
    vertices[globalID].color.b *= 0.0;
    vertices[globalID].color.a *= 1.0;

    vertices[globalID].position.x *= 5.0;
}
