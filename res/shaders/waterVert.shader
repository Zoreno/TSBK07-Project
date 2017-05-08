#version 450 core

layout (location = 0) in vec3 position;

out vec4 clipSpace;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	clipSpace = projection*view*model*vec4(position, 1.f);
	
	gl_Position = clipSpace;
}