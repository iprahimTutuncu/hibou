#version 430 core
out float fColor;

in vec4 vColor;
in vec2 vTexCoord;

uniform sampler2D ssao;
uniform float screen_width;
uniform float screen_height;
uniform int blur_radius;

void main()
{
    vec2 texelSize = 1.0 / vec2(screen_width, screen_height);
    
    float color = 0.0;

    for (int i = -blur_radius; i <= blur_radius; i++)
        for (int j = -blur_radius; j <= blur_radius; j++) {
            vec2 offset = vec2(i, j) * texelSize;
            color += texture(ssao, vTexCoord + offset).r;
        }
    
    color /= float(pow(2 * blur_radius + 1, 2));

    fColor = color;
}
