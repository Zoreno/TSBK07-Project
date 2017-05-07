#version 450 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D hdrBuffer;
uniform sampler2D bloomBuffer;
uniform bool hdr;
uniform bool bloom;
uniform float exposure;
uniform float gamma;

void main()
{
	vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
	vec3 bloomColor = texture(bloomBuffer, TexCoords).rgb;
	
	if(bloom)
		hdrColor += bloomColor;
	
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
	result = pow(result, vec3(1.0/gamma));
	
	color = vec4(result, 1.0f);
	
	
}