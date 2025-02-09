#version 430

// Define the layout of the output image
layout(binding = 0, rgba8) uniform writeonly image2D destAlbedoTex;
layout(binding = 1, r32f) uniform writeonly image2D destDepthTex;
layout(binding = 2, rgba32f) uniform writeonly image2D destPositionTex;
layout(binding = 3, rgba32f) uniform writeonly image2D destNormalTex;

layout(binding = 4, rgba8) uniform readonly image2D albedo;
layout(binding = 5, rgba8) uniform readonly image2D heightmap;

// Uniforms
uniform vec3 cameraPosition;
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
uniform int blurType;

// Define the work group size
layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

void main()
{
    // Get the global ID of the thread
    int i = int(gl_GlobalInvocationID.x);

    float z = 1.0;
    float dz = 0.9;
    float half_size = float(terrainSize) * 0.5;
    float current_y = screenHeight;

    float sinphi = sin(angle);
    float cosphi = cos(angle);

    float invScreenWidth = 1.0 / screenWidth;
    float invScreenHeight = 1.0 / screenHeight;

    float invFOV = invScreenWidth; // avoir un field of view different de l'ecran ramene a du trouble, specifiquement avec les sprites, a mediter
    float invTerrainRenderDistance = 1.0 / terrainRenderDistance;

    while (z < terrainRenderDistance) {

        vec2 offset = cameraPosition.xz + half_size;
        vec2 pleft = vec2(-cosphi * z - sinphi * z, sinphi * z - cosphi * z) + offset;
        vec2 pright = vec2(cosphi * z - sinphi * z, -sinphi * z - cosphi * z) + offset;

        float dx = (pright.x - pleft.x) * invFOV;
        float dy = (pright.y - pleft.y) * invFOV;

        pleft += vec2(dx, dy) * i;

        if (pleft.x > 0 && pleft.y > 0 && pleft.x < terrainSize && pleft.y < terrainSize) 
        {
            ivec2 pixel_indice = ivec2(pleft);
            float curr_height = imageLoad(heightmap, pixel_indice).r * scaleHeight;
            float height_on_screen = (cameraPosition.y - curr_height) / z * scaleHeight + horizon;


            if (current_y > height_on_screen)
            {
                float curr_height_left = imageLoad(heightmap, ivec2(pixel_indice.x - 1, pixel_indice.y)).r * scaleHeight;
                float curr_height_right = imageLoad(heightmap, ivec2(pixel_indice.x + 1, pixel_indice.y)).r * scaleHeight;
                float curr_height_up = imageLoad(heightmap, ivec2(pixel_indice.x, pixel_indice.y + 1)).r * scaleHeight;
                float curr_height_down = imageLoad(heightmap, ivec2(pixel_indice.x, pixel_indice.y - 1)).r * scaleHeight;

                // Compute gradients in the x and y directions
                float gradientX = (curr_height_left - curr_height_right); // Gradient in x direction
                float gradientY = (curr_height_down - curr_height_up);   // Gradient in y direction

                // Compute the normal vector
                vec3 normal = normalize(vec3(gradientX, 1.0 / scaleHeight, gradientY)); // Assuming depth component is 1.0

                vec4 albedo_color = imageLoad(albedo, pixel_indice);

                for (int j = int(current_y); j > int(height_on_screen) && j > 0.f; j--)
                {

                    imageStore(destAlbedoTex, ivec2(i, j), albedo_color);
                    imageStore(destDepthTex, ivec2(i, j), vec4(z * invTerrainRenderDistance));

                    float normalizedX = (2.0 * (float(i) * invScreenWidth)) - 1.0;
                    float normalizedY = 1.0 - (2.0 * (float(j) * invScreenHeight));

                    //imageStore(destPositionTex, ivec2(i, j), vec4(normalizedX, normalizedY, z / terrainRenderDistance, 1.0)); //screen space

                    float xPos = float(pixel_indice.x) - half_size;
                    float yPos = curr_height;
                    float zPos = float(pixel_indice.y) - half_size;


                    imageStore(destPositionTex, ivec2(i, j), vec4(xPos, yPos, zPos, 1.0));

                    //    TODO, have a version that use a precomputed version. Normal map calculation doubles the calculation due to the sampling.
                    //  Hench, this calculation doesn't even do the blurring of the normal, it might be even more expensive. If it's not in realtime
                    //  might have the normal be calculated elsewhere and sampled once here. Might also enable precompute of the blurr. Both have to be option
                    //  with real-time moving blur I can mimick grass. Multiple option are to consider. For now it's all realtime to the worst possible performance

                    // Grid spacing (distance between adjacent height samples)
                    float gridSpacing = 1.0; // Adjust this value as needed for your grid

                    imageStore(destNormalTex, ivec2(i, j), vec4(normal, 1.0));
                }

                current_y = height_on_screen;
            }
        }

        z += dz;
        if (z > LODStartAt) dz += LOD;
    }

}
