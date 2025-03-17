#version 460

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, rgba8) readonly uniform image2D positionMap;
layout(binding = 1, rgba8) readonly uniform image2D albedoMap;
layout(binding = 2, rgba32f) readonly uniform image2D materialMap;
layout(binding = 3, rgba32f) writeonly uniform image2D outputImage;

struct Material
{
    vec4 albedo;
    float roughness;
    float padding1;
    vec4 emission;
    int isEmitter;
    int padding2, padding3, padding4;
};

layout(binding = 4, std430) buffer Materials
{
    Material materials[32];
};


uniform vec2 screenSize;
uniform vec4 cameraIntrinsics;

struct Ray
{
    vec3 origin;
    vec3 direction;
};

Ray generateRayWithCameraIntrinsics(vec2 pixelCoord, vec2 screenSize, vec4 cameraIntrinsics)
{
    float fx = cameraIntrinsics.x;
    float fy = cameraIntrinsics.y;
    float cx = cameraIntrinsics.z;
    float cy = cameraIntrinsics.w;

    vec2 sensorCoord = vec2
    (
        (pixelCoord.x - cx) / fx,
        (pixelCoord.y - cy) / fy
    );

    vec3 rayDir = normalize(vec3(sensorCoord, -1.0));
    vec3 rayOrigin = vec3(0.0, 0.0, 0.0); // Camera at origin

    return Ray(rayOrigin, rayDir);
}

bool rayHitsGeometry(Ray ray, vec2 pixelCoord)
{
    const float maxDistance = 1000.0;  // Maximum march distance
    const float stepSize = 0.1;      // Step size per iteration
    const int maxSteps = 10000;        // Max number of raymarching steps

    float distanceTraveled = 0.0;
    vec3 currentPos = ray.origin;

    for (int i = 0; i < maxSteps; i++)
    {
        ivec2 texCoord = ivec2(currentPos.xy); // Project to 2D pixel space
        vec3 hitPosition = imageLoad(positionMap, texCoord).xyz;

        // If we hit valid geometry, return true
        if (length(hitPosition) > 0.1)
        {
            return true;
        }

        // March further along the ray
        currentPos += ray.direction * stepSize;
        distanceTraveled += stepSize;

        // Stop if we exceed the max distance
        if (distanceTraveled > maxDistance)
        {
            break;
        }
    }

    return false; // No hit found
}

Ray generateRay(vec2 pixelCoord, vec2 screenSize)
{
    vec2 ndc = (pixelCoord / screenSize) * 2.0 - 1.0;

    vec3 rayDir = normalize(vec3(ndc, -1.0));
    vec3 rayOrigin = vec3(0.0, 0.0, 0.0);

    return Ray(rayOrigin, rayDir);
}


// Main function that runs per workgroup
void main()
{
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);

    Ray ray = generateRay(vec2(pixelCoord), screenSize);

    ivec2 texCoord = ivec2((ray.origin.xy * 0.5 + 0.5) * screenSize); // Convert from NDC to screen space

    // bool hit = rayHitsGeometry(ray, vec2(pixelCoord));

    ivec2 screenCoord = texCoord; // Ensure this maps correctly to your positionMap
    vec3 hitPosition = imageLoad(positionMap, screenCoord).xyz;

    bool hit = hitPosition.z > 0.0f;


    if (hit)
    {
        vec3 albedo = imageLoad(albedoMap, pixelCoord).xyz;
        imageStore(outputImage, pixelCoord, vec4(albedo, 1.0));
    }
    else
    {
        imageStore(outputImage, pixelCoord, vec4(texCoord, 0.0, 1.0));
    }

    imageStore(outputImage, pixelCoord, vec4(screenCoord/screenSize, 0.0, 1.0));



}
