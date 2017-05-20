#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coordinates;

out vec3 normal;
out vec3 FragPos;
out vec2 texCoords;

uniform mat4 transform;
uniform mat4 model;
uniform float time;

void main()
{
	normal = mat3(transpose(inverse(model)))*vertex_normal;

	gl_Position = transform*vec4(vertex_position, 1.0);
	FragPos = vec3(model*vec4(vertex_position, 1.0f));
	texCoords = vertex_texture_coordinates;
}