#version 430 core
out vec4 fColor;

in vec4 vColor;
in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D default_texture;
uniform sampler2D gObjectPicking;
uniform sampler2D gLocalNormal;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform sampler2D gSpecular;
uniform sampler2D ssao;
uniform sampler2D shadowMap;

uniform int hover_id;
uniform int selected_id;
uniform vec3 hover_normal;
uniform vec3 eyePosition;
uniform vec3 eyeDirection;
uniform float time;
uniform mat4 view;
uniform mat4 projection;

uniform bool debug_show_diffuse;
uniform bool debug_show_normal;
uniform bool debug_show_position;
uniform bool debug_show_specular;
uniform bool debug_show_ambiant;
uniform bool debug_show_shadow;

struct DirectionLight {
    vec3 ambiant;

    vec3 color;
    vec3 direction;
    bool isActive;
};

uniform DirectionLight directionLights[5];

int unpackRGBAToInt(vec4 color) {
    int r = int(color.r * 255.0);
    int g = int(color.g * 255.0);
    int b = int(color.b * 255.0);
    int a = int(color.a * 255.0);
    return (r) | (g << 8) | (b << 16) | (a << 24);
}

uniform float num_samples = 3;
uniform float density = 0.1;
uniform float weight = 0.1;
uniform float decay = 0.1;
uniform float exposure = 1.;

void main()
{
    vec4 texColor = texture(default_texture, vTexCoord);
    int objectPicked = unpackRGBAToInt(texture(gObjectPicking, vTexCoord));
    vec3 normalPicked = texture(gLocalNormal, vTexCoord).xyz;

    //code pour chercher la position de la lumière à l'écran
    vec4 lightDirView = view * vec4(-directionLights[0].direction, 0.0);
    vec4 lightDirClip = projection * lightDirView;
    vec3 lightDirNDC = lightDirClip.xyz / lightDirClip.w;

    vec2 screenLightPos = 0.5 * (lightDirNDC.xy + 1.0);
    
    //code pour éviter les angles extrème où la densité casse
    float attenuated_density = density;
    attenuated_density *= abs(1.0 / max(abs(screenLightPos.x), abs(screenLightPos.y)));

    /*
    * Code provenant de la section "Example 13-1. Post-Process Shader Implementation of Additive Sampling"
    * avec petite modification mis-à part l'adaptation vers GLSL. Dans la partie "Retrieve sample at new location."
    * je fais un test d'occlusion avec un texture des normalLocals. C'est pour m'éviter de me faire une autre pass pour un test
    *
    * https://developer.nvidia.com/gpugems/gpugems3/part-ii-light-and-shadows/chapter-13-volumetric-light-scattering-post-process
    */

    // Calculate vector from pixel to light source in screen space.
    vec2 deltaTexCoord = (vTexCoord - screenLightPos);

    // Divide by number of samples and scale by control factor.
    deltaTexCoord *= 1.0 / num_samples * attenuated_density;

    // Set up illumination decay factor.
    float illuminationDecay = 1.0;

    vec2 texCoord = vTexCoord;
    vec3 color = vec3(0.0);
    // Check if the pixel is near the screenLightPos and set it to red if true.
    float distanceToLight = length(deltaTexCoord);

    for (float i = 0.0; i < num_samples; i++) {
        // Step sample location along ray.
        texCoord -= deltaTexCoord;

        // Retrieve sample at new location.
        float occluded = texture(gLocalNormal, texCoord).a;
        vec3 s;
        if(occluded != 0.)
            s = vec3(0.0);
        else
            s = texture(default_texture, texCoord).rgb;

        // Apply sample attenuation scale/decay factors.
        s *= illuminationDecay * weight;

        // Accumulate combined color.
        color += s;

        // Update exponential decay factor.
        illuminationDecay *= decay;
    }
    

    // Output final color with a further scale control factor.
    texColor = texColor + vec4(color * exposure, 1.0);

    /*
    * reinhard tone mapping pris de la section
    * Tone mapping, ce code. Seul différence cE'st que j'ai mis 
    * ma propre valeur d'exposure en uniform.
    *
    * https://learnopengl.com/Advanced-Lighting/HDR
    */

    float gamma = 2.2;

    vec3 hdrColor = texColor.rgb;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));
    texColor.rgb = mapped;


    if(objectPicked == selected_id)
        texColor += vec4(0.5, 0.1, 0.0, 0.0);

    if(normalPicked == hover_normal && hover_id == objectPicked)
        texColor += vec4(0.3, 0.2, 0.0, 0.0);

    fColor = texColor;

    if(debug_show_diffuse)
        fColor = vec4(texture(gColor, vTexCoord).rgb, 1.0);
    if(debug_show_normal)
        fColor = vec4(texture(gNormal, vTexCoord).rgb, 1.0);
    if(debug_show_position)
        fColor = vec4(texture(gPosition, vTexCoord).rgb, 1.);
    if(debug_show_specular)
        fColor = vec4(texture(gSpecular, vTexCoord).rgb, 1.);
     if(debug_show_ambiant)
        fColor = vec4(vec3(texture(ssao, vTexCoord).r), 1.0);
     if(debug_show_shadow)
        fColor = vec4(vec3(texture(shadowMap, vTexCoord).r), 1.0);
}
