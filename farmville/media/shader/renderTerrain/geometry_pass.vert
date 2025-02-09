#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec4 color;
layout (location = 6) in vec3 externalPosition; // change this for extBound

uniform mat4 projection;         // Projection matrix (for orthographic projection)
uniform mat4 view;               // View matrix (for camera transformations)
uniform mat4 model;
uniform mat4 transform;
uniform mat4 rotation;


uniform float scaleHeight;

out float vDepth;
out vec2 vTexCoord;
out vec4 vColor; 
out vec3 vPosition;

uniform vec2 extScale;
uniform vec3 extPosition;
uniform float extRotation;

const float PI = 3.141592653589793;

void main() {
    // Combine transformation matrices
    mat4 M  =  model * transform;
    mat4 MV  = view * model * transform;
    mat4 MVP = projection * MV;

    // Transform position
    vec4 transformedPosition = MVP * vec4(position, 1.0);
    gl_Position = transformedPosition;
    
    // Calculate fragment depth
    vDepth = (MV * vec4(position, 1.0)).z;

    // Pass other variables
    vTexCoord = texCoord;
    vColor = color;


    //check this junk on renderdoc, check if uniform are sane
    //enjoy and have fun

    vPosition = (vec4(externalPosition, 1.0)).xyz;
}
