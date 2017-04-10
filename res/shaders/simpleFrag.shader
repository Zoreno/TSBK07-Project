#version 450 core

in vec3 normal;
in vec3 FragPos;
in vec2 texCoords;

out vec4 frag_color;

void main()
{
	frag_color = vec4(normal, 1.0);
}