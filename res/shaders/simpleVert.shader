#version 450 core

//=============================================================================
// Input
//=============================================================================

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coordinates;

//=============================================================================
// Output
//=============================================================================

out vec3 normal;
out vec3 FragPos;
out vec2 texCoords;

//=============================================================================
// Uniforms
//=============================================================================

uniform mat4 transform;
uniform mat4 model;
uniform vec4 clippingPlane;

//=============================================================================
// Functions
//=============================================================================

//=============================================================================
// Main
//=============================================================================

void main()
{

	const vec4 pos = transform*vec4(vertex_position, 1.0);

	gl_ClipDistance[0] = dot(pos, clippingPlane);
	
	// Invert-transpose model to find normal transformations
	normal = mat3(transpose(inverse(model)))*vertex_normal;
	
	// Output Vertex position
	gl_Position = pos;
	
	// Pass Fragment position
	FragPos = vec3(model*vec4(vertex_position, 1.0));
	
	// Pass Texture Coordinates
	texCoords = vertex_texture_coordinates;
}

//=============================================================================
// End of file
//=============================================================================