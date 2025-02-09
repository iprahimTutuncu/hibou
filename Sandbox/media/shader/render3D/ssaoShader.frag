#version 330 core
out float FragColor;
  
in vec2 vTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D textureNoise;

uniform vec3 samples[64];
uniform mat4 view;
uniform mat4 projection;

uniform float screen_width;
uniform float screen_height;

int kernelSize = 64;
uniform float radius;

/*
	Le code est une adaptation quasi identique d'un mélange de ces deux sources
	http://john-chapman-graphics.blogspot.com/2013/01/ssao-tutorial.html
	https://learnopengl.com/Advanced-Lighting/SSAO

	j'utilise le range check & accumulation comme dans le blog de jonh chapman
	et comme dans le tutoriel de learnopenGL, j'utilise la position map pour
	échantionner la profondeur.
*/

void main(){
	vec2 noiseScale = vec2(screen_width/4.0, screen_height/4.0);
	vec4 fragPosition = texture(gPosition, vTexCoord);
	vec3 position   = (view * fragPosition).xyz;
	vec3 normal    = (transpose(inverse(view)) * texture(gNormal, vTexCoord)).xyz;
	vec3 rvec = texture(textureNoise, vTexCoord * noiseScale).xyz;

	//processus grand-schmit pour calculé TBN
	vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 tbn = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	for (int i = 0; i < kernelSize; ++i) {

	   vec3 curr_sample = tbn * samples[i];
	   curr_sample = curr_sample * radius + position;
  
	   vec4 offset = vec4(curr_sample, 1.0);
	   offset = projection * offset;
	   offset.xy /= offset.w;
	   offset.xy = offset.xy * 0.5 + 0.5;
  
	   float sampleDepth = (view * texture(gPosition, offset.xy)).z;
  
		float rangeCheck = abs(position.z - sampleDepth) < radius ? 1.0 : 0.0;
		occlusion += (sampleDepth >= curr_sample.z ? 1.0 : 0.0) * rangeCheck;
	}

	 occlusion = 1.0 - (occlusion / kernelSize);
	 FragColor = occlusion;
}