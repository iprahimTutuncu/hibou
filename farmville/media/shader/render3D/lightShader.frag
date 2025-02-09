#version 400 core
layout (location = 0) out vec4 fColor;

in vec4 vColor;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 eyePosition;
uniform vec3 eyeDirection;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightView;
uniform mat4 lightProjection;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform sampler2D gSpecular;
uniform sampler2D ssao;
uniform sampler2D shadowMap;

uniform float maxShadowBias = 0.025;
uniform float minShadowBias = 0.0001;
uniform float exponentShadowBias = 1.0;

struct DirectionLight {
    vec3 color;
    vec3 direction;
    bool isActive;
};

struct PointLight{
    vec3 color;
    vec3 position;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
    bool isActive;
};

struct SpotLight{
    vec3 color;
    vec3 position;
    vec3 direction;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
    float cutOff;
    bool isActive;
};

uniform DirectionLight directionLights[5];
uniform PointLight pointLights[20];
uniform SpotLight spotLights[20];

struct Ray{
    vec3 ori;
    vec3 dir;
};

struct Sphere{
    vec3 center;
    float radius;
};

// dans le calcul d'intersection je n'utilise pas la position, la sphere
// est toujours centré à l'origine vec3(0.0, 0.0, 0.0)
bool raySphereIntersect(Ray r, float radius, inout float t0, inout float t1){
    float t = dot(-r.ori, r.dir);
    vec3 p = r.ori + r.dir*t;
    float y = length(p);
    if(y < radius){
        float x = sqrt(radius * radius - y*y);
        t0 = t-x;
        t1 = t+x;
        return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Code de la méthode computeIncidentLight() est une adaptation qui provient du tutoriel de scratch a pixel avec certaine
// modification. Les valeurs global en dessous de ce commentaire sont les valeur que dépendent cette même méthode, elles
// aussi du tutoriel
// https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/simulating-sky/simulating-colors-of-the-sky.html
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uniform float earthRadius = 6360e3;
uniform float atmosphereRadius = 6420e3;
uniform float Hr = 7994.0;
uniform float Hm = 1200.0;
const float PI = radians(180.0);
uniform vec3 betaR = vec3(3.8e-6, 13.5e-6, 33.1e-6);
uniform vec3 betaM = vec3(21e-6);
uniform vec3 atmospherePosition = vec3(0.0);
const float kInfinity = uintBitsToFloat(0x7F800000);
uniform float Esun = 20.;

vec3 computeIncidentLight(vec3 sunDirection, Ray r, float tmin, float tmax)
{ 
    float betaMieAttenuation = 1.;
    float attenuation = 1.;

    if(r.dir.y < 0.0){
        r.dir.y = -r.dir.y;
        attenuation *= 0.3;
        betaMieAttenuation = 0.;
    }

    float t0, t1;
    if (!raySphereIntersect(r, atmosphereRadius, t0, t1) || t1 < 0.0) return vec3(0.0); 
    if (t0 > tmin && t0 > 0.) tmin = t0; 
    if (t1 < tmax) tmax = t1; 
    int numSamples = 16; 
    int numSamplesLight = 8; 
    float segmentLength = (tmax - tmin) / float(numSamples); 
    float tCurrent = tmin; 
    vec3 sumR = vec3(0.);
    vec3 sumM = vec3(0.);
    float opticalDepthR = 0.;
    float opticalDepthM = 0.; 
    


    float mu = dot(r.dir, sunDirection);
    float phaseR = 3.f / (16.f * PI) * (1.0 + mu * mu); 
    float g = 0.76f; 
    float phaseM = 3.f / (8.f * PI) * ((1.f - g * g) * (1.f + mu * mu)) / ((2.f + g * g) * pow(1.f + g * g - 2.f * g * mu, 1.5f));



    for (int i = 0; i < numSamples; ++i) { 
        vec3 samplePosition = r.ori + (tCurrent + segmentLength * 0.5f) * r.dir; 
        float height = length(samplePosition) - earthRadius; 
        // compute optical depth for light
        float hr = exp(-height / Hr) * segmentLength; 
        float hm = exp(-height / Hm) * segmentLength; 
        opticalDepthR += hr; 
        opticalDepthM += hm; 
        // light optical depth
        float t0Light, t1Light;

        Ray r;
        r.ori = samplePosition;
        r.dir = sunDirection;

        raySphereIntersect(r, atmosphereRadius, t0Light, t1Light); 
        float segmentLengthLight = t1Light / numSamplesLight;
        float tCurrentLight = 0.0; 
        float opticalDepthLightR = 0.0;
        float opticalDepthLightM = 0.0; 
        int j; 
        for (j = 0; j < numSamplesLight; ++j) { 
            vec3 samplePositionLight = samplePosition + (tCurrentLight + segmentLengthLight * 0.5f) * sunDirection; 
            float heightLight = length(samplePositionLight) - earthRadius; 
            if (heightLight < 0.0) break; 
            opticalDepthLightR += exp(-heightLight / Hr) * segmentLengthLight; 
            opticalDepthLightM += exp(-heightLight / Hm) * segmentLengthLight; 
            tCurrentLight += segmentLengthLight; 
        }
        if (j == numSamplesLight) { 
            vec3 tau = betaR * (opticalDepthR + opticalDepthLightR) + betaM * betaMieAttenuation * 1.1f * (opticalDepthM + opticalDepthLightM); 
            vec3 attenuation = vec3(exp(-tau.x), exp(-tau.y), exp(-tau.z)); 
            sumR += attenuation * hr; 
            sumM += attenuation * hm; 
        } 
        tCurrent += segmentLength; 
    }
 
    return (sumR * betaR * phaseR + sumM * betaM * phaseM) * Esun * attenuation;
} 


/////////////////////////////////////////////////////////////////////////
// inspiré mais pas pareil, beaucoup de modification au lab3
// dû à la carte atmospherique. Reste très similaire toutefois.
// de openGL programming guide, eight edition (redbook), p.367
/////////////////////////////////////////////////////////////////////////

vec4 directional_light_blinn(DirectionLight dirlight, vec3 position, vec3 normal, vec3 specular, vec3 ambiant, float shininess, vec3 color)
{
    vec3 lightDirection = normalize(dirlight.direction);
    vec3 viewDirection = normalize(eyePosition - position);

    vec3 halfVector = normalize(lightDirection + viewDirection);

    float diff = max(0.0, dot(normal, lightDirection));

    float spec = pow(max(dot(normal, halfVector), 0.0), shininess);

    Ray r;
    r.ori = vec3(atmospherePosition.x, earthRadius + atmospherePosition.y, atmospherePosition.z) + position;
    r.dir = normal;


    vec3 envColor = vec3(0.0);

    vec3 up = vec3(0.0, 1.0, 0.0);
    if (up == -lightDirection)
        up = vec3(1.0, 0.0, 0.0);

    vec3 right = normalize(cross(lightDirection, up));
    up = cross(lightDirection, right);
    float radius = 0.1;
    for (float i = 0; i < 8; i += 1) {
        float angle = i * (2.0 * PI / 4.0);
        
        vec3 sampleDirection = radius * normalize(cos(angle) * right + sin(angle) * up);
        r.dir = normalize(normal + sampleDirection);

        envColor += (1.0/8.0) * computeIncidentLight(lightDirection, r, 0.0, kInfinity);
    }


   /*
    * reinhard tone mapping pris de la section
    * Tone mapping, ce code.
    *
    * https://learnopengl.com/Advanced-Lighting/HDR
    */

    //on rajoute un tonemapping parceque la couleur "envColor" est en HDR
    float gamma = 2.2;
    vec3 hdrColor = envColor;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * 1.0);
    envColor = pow(mapped, vec3(1.0 / gamma));

    mat4 viewProjLight = lightProjection * lightView;

    vec4 shadowCoord = lightProjection * lightView * vec4(position, 1.0);
    vec3 coord = 0.5*(shadowCoord.xyz / shadowCoord.w)+0.5;
    float visible = 0.0;
    for(float i = -1.0; i < 1.0; i++)
        for(float j = -1.0; j < 1.0; j++){
            vec2 offset = vec2(i/1024.0, j/1024.0);
            float shadowMapDepth = texture(shadowMap, coord.xy + offset).r; 
            float fragmentDepth = coord.z;

            float linearBias = max(maxShadowBias * (1.0 - dot(normal, lightDirection)), minShadowBias);
            float bias = pow(linearBias, exponentShadowBias);
            bias = max(bias, minShadowBias);
            visible += fragmentDepth <= shadowMapDepth + bias  ? 1.0 : 0.0;
    }
    visible /= 9.0;

    //scatter + reflective
    vec3 scattered_light = envColor * ambiant + visible * envColor * color * diff;
    vec3 reflective_light = visible * envColor * specular * spec;

    vec3 result = scattered_light + reflective_light;

    return vec4(clamp(result, 0.0, 1.0), 0.0);
}


/////////////////////////////////////////////////////////////////////////
// inspiré mais pas pareil
// de openGL programming guide, eight edition (redbook), p.369
/////////////////////////////////////////////////////////////////////////

vec4 point_light_blinn(PointLight pointLight, vec3 position, vec3 normal, vec3 specular, float shininess, vec3 diffuse)
{
    vec3 lightDirection = normalize(pointLight.position - position);
    vec3 viewDirection = normalize(eyePosition - position);
    
    vec3 halfVector = normalize(lightDirection + viewDirection);

    float diff = max(0.0, dot(normal, lightDirection));
    float spec = pow(max(dot(normal, halfVector), 0.0), shininess);

    float lightDistance = length(pointLight.position - position);
    float attenuation = 1.0 / (pointLight.attenuationConstant + pointLight.attenuationLinear * lightDistance + pointLight.attenuationQuadratic * lightDistance * lightDistance);

    vec3 scattered_light = pointLight.color * diff * diffuse * attenuation;
    vec3 reflective_light = pointLight.color * spec * specular * attenuation;

    vec3 result = scattered_light + reflective_light;

    return vec4(clamp(result, 0.0, 1.0), 0.0);
}

/////////////////////////////////////////////////////////////////////////
// inspiré mais pas pareil
// de openGL programming guide, eight edition (redbook), p.369
/////////////////////////////////////////////////////////////////////////

vec4 spot_light_blinn(SpotLight spotLight, vec3 position, vec3 normal, vec3 specular, float shininess, vec3 diffuse)
{
    vec3 lightDirection = normalize(spotLight.position - position);
    vec3 viewDirection = normalize(eyePosition - position);
    
    vec3 halfVector = normalize(lightDirection + viewDirection);

    float diff = max(0.0, dot(normal, lightDirection));
    float spec = pow(max(dot(normal, halfVector), 0.0), shininess);



    float lightDistance = length(spotLight.position - position);
    float attenuation = 1.0 / (spotLight.attenuationConstant + spotLight.attenuationLinear * lightDistance + spotLight.attenuationQuadratic * lightDistance * lightDistance);
    float spotCos = dot(lightDirection, -spotLight.direction);
    if (spotCos < spotLight.cutOff)
        attenuation = 0.0;

    vec3 scattered_light = spotLight.color * diff * diffuse * attenuation;
    vec3 reflective_light = spotLight.color * spec * specular * attenuation;

    vec3 result = scattered_light + reflective_light;
    return vec4(clamp(result, 0.0, 1.0), 0.0);
}

void main()
{
    vec3 position = texture(gPosition, vTexCoord).xyz;
    vec3 normal = normalize(texture(gNormal, vTexCoord).xyz);
    vec3 color = texture(gColor, vTexCoord).rgb;
    vec3 specular = texture(gSpecular, vTexCoord).rgb;
    float shininess = texture(gSpecular, vTexCoord).a * 256;
    float ambient_occlusion = texture(ssao, vTexCoord).r;
    vec3 ambiant = ambient_occlusion * color;
    vec4 result = vec4(0.0, 0.0, 0.0, 1.0);

    if(texture(gPosition, vTexCoord).xyz == vec3(0.0)){
        
        //ramener la position en espace monde
        vec4 ndc = vec4((vTexCoord * 2.0 - 1.0), 0.0, 1.0);
        mat4 invProj = inverse(projection);
        vec4 viewCoords = invProj * ndc;

        viewCoords.z = -1.0;
        viewCoords.w = 0.0;

        mat4 invView = inverse(view);
        vec4 worldCoords = invView * viewCoords;

        //si on normalize le world coord, essentiellement de quoi qui map une sphere
        vec3 normal = normalize(worldCoords.xyz);
      
        Ray r;
        r.ori = vec3(atmospherePosition.x, earthRadius + atmospherePosition.y, atmospherePosition.z);
        r.dir = normal;

        vec3 envMap = computeIncidentLight(directionLights[0].direction, r, 0.0, kInfinity);

        // Output the final color
        result = vec4(envMap, 1.0);

    }else{
        for (int i = 0; i < 5; i++)
                if (directionLights[i].isActive)
                    result += directional_light_blinn(directionLights[i], position, normal, specular, ambiant, shininess, color);
                else
                    break;

        for (int i = 0; i < 20; i++) {
            if (pointLights[i].isActive)
                result += point_light_blinn(pointLights[i], position, normal, specular, shininess, color);
            else
                break;
        }

        for (int i = 0; i < 20; i++) {
            if (spotLights[i].isActive)
                result += spot_light_blinn(spotLights[i], position, normal, specular, shininess, color);
            else
                break;
        }
    }

    fColor = result;
}
