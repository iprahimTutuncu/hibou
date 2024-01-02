#version 430 core
out vec4 fColor;

in vec4 vColor;
in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D gLocalNormal;
uniform sampler2D gObjectPiking;
uniform float maximumLocalSize;

int unpackRGBAToInt(vec4 color) {
    int r = int(color.r * 255.0);
    int g = int(color.g * 255.0);
    int b = int(color.b * 255.0);
    int a = int(color.a * 255.0);
    return (r) | (g << 8) | (b << 16) | (a << 24);
}

void main()
{
    vec3 normal_color = texture(gLocalNormal, vTexCoord).rgb;
    vec4 objtPick_color = texture(gObjectPiking, vTexCoord);

    int objectPickID = unpackRGBAToInt(objtPick_color);

    fColor = vec4(normal_color,float(objectPickID));
}
