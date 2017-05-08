#version 450 core

in vec4 clipSpace;

out vec4 color;

uniform sampler2D refractionTexture;
uniform sampler2D reflectionTexture;

void main()
{
	// Normalized device space coordinates
	vec2 ndc = ((clipSpace.xy / clipSpace.w) / 2.0) + 0.5;
	
	vec2 refractTextureCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTextureCoords = vec2(ndc.x, -ndc.y);
	
	vec4 refractionColor = texture(refractionTexture, refractTextureCoords);
	vec4 reflectionColor = texture(reflectionTexture, reflectTextureCoords);
	
	
	color = mix(reflectionColor, refractionColor, 0.5);
	//color = reflectionColor;

	//color = refractionColor;
	
	//color = vec4(0.0,0.0,1.0,1.0);
}