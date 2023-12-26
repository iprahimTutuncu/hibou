#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec4 color;

uniform mat4 projection;               // Projection matrix (for orthographic projection)
uniform mat4 view;                     // View matrix (for camera transformations)
uniform mat4 model;                    
uniform mat4 transform;                    

out vec2 fragment_texCoord;                    // Output variable to pass texture coordinates to fragment shader
out vec4 vertex_color;                       // Output variable to pass color to fragment shader

void main() {

    gl_Position = projection * view * model * transform * vec4(position.xy, 0.0 , 1.0);

    fragment_texCoord = texCoord.xy;

    vertex_color = color;
}
