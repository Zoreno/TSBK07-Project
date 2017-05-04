#version 450 core

in vec3 normal;
in vec3 FragPos;
in vec2 texCoords;

out vec4 frag_color;

uniform sampler2D tex;

void main()
{
	vec4 objColor = texture(tex, texCoords);

	frag_color = objColor;
}