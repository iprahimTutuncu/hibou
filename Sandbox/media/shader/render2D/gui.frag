#version 430 core

in vec2 vTexCoord; 
in vec4 vColor; // Color from the vertex shader
out vec4 fColor;

// Uniforms for texture and key color
uniform vec4 keyColor;
uniform sampler2D textureDefault;

// Uniforms for border and shadow
uniform vec4 borderColor1; // Color of the border
uniform vec4 borderColor2; // Color of the shadow
uniform vec4 borderColor3; // Color of the shadow
uniform vec4 backgroundColor; // Color of the shadow
uniform float borderWidth; // Thickness of the border in normalized coordinates
uniform float cornerRadius; // Radius for rounded corners
uniform vec2 panelSize; // Size of the panel (for rounded corners)
uniform float shadowSize; // Size of the shadow spread
uniform int hasBgTexture;
uniform int isText;

// from http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float udRoundBox( vec2 p, vec2 b, float r )
{
    return abs(length(max(abs(p)-b+r,0.0)))-r;
}

float nearestSeq(float val, float seq){
	float multiple = val/seq;
	float fractMultipe = fract(multiple);
	multiple -= fractMultipe;
	fractMultipe = round(fractMultipe);
	multiple += fractMultipe;
	return multiple * seq;
}

void main()
{
    // Sample the texture color
    vec4 color = backgroundColor;

    if(hasBgTexture > 0)
    {
        color = texture(textureDefault, vTexCoord);
        if (color == keyColor)
            discard;
    }

    if (isText > 0)
    {
        // Render as text, bypassing border and shadow effects
        fColor = color * vColor;
    }
    else
    {
        //position is pixelized
        vec2 half_size = panelSize * 0.5;
        float r = min(panelSize.x, panelSize.y);
        vec2 position;
        position.x = panelSize.x*nearestSeq(vTexCoord.x, 0.0005) - half_size.x;
        position.y = panelSize.y*nearestSeq(vTexCoord.y, 0.0005) - half_size.y;

        // Calculate distance from the fragment to the border
        float dist = udRoundBox(position, half_size - vec2(borderWidth), cornerRadius);

         // Distances for different borders
        float border1 = udRoundBox(position, half_size - vec2(borderWidth), cornerRadius);
        float border2 = udRoundBox(position, half_size - vec2(borderWidth * 0.5), cornerRadius);
        float border3 = udRoundBox(position, half_size - vec2(borderWidth * 0.25), cornerRadius);

        // Output final color with mixed borders
        fColor = mix(borderColor1, color , smoothstep(1.0, 0.0, border1));
        fColor = mix(borderColor2, fColor, smoothstep(1.0, 0.0, border2));
        fColor = mix(borderColor3,fColor, smoothstep(1.0, 0.0, border3));

        fColor.a = 1.0;

    }
}
