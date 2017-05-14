#version 330 core

in vec3 normal;
in vec3 FragPos;
in vec2 texCoords;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

uniform float time;

uniform sampler2D texUnit;

void main()
{
	vec4 objColor = texture(texUnit, texCoords);

	fragColor = objColor.zyxw;
	brightColor = vec4(0.f,0.f,0.f,0.f);
}