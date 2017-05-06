#version 450 core

//=============================================================================
// Structs
//=============================================================================

struct Light
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

//=============================================================================
// Input
//=============================================================================

in vec3 normal;
in vec3 FragPos;
in vec2 texCoords;

//=============================================================================
// Output
//=============================================================================

out vec4 fragColor;

//=============================================================================
// Uniforms
//=============================================================================

// Texture to sample main texture from.
uniform sampler2D 	textureUnit;

// Position of the camera during the render pass.
uniform vec3 		viewPos;

// Number of lights in scene
uniform int numLights;

// Light data. TODO: Variable length buffer with SSBO
uniform Light lights[64];

// Material Properties
uniform Material material;

//=============================================================================
// Functions
//=============================================================================

float calculateAttenuation(float distance, Light light)
{
	return 1.0f/(light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0f));
}

//=============================================================================
// Main
//=============================================================================

void main()
{
	// Calculate useful variables
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// Variable to store light result.
	vec3 resColor = vec3(0,0,0);
	for(int i = 0; i < numLights; ++i)
	{
		vec3 lightDirection = normalize(lights[i].position - FragPos);

		// Ambient light calculation
		vec3 ambientColor = lights[i].ambient*material.ambient;
	
		// Diffuse light calculation
		float diff = max(dot(norm, lightDirection), 0.0);
		vec3 diffuseColor = lights[i].diffuse * diff * material.diffuse;
	
		// Specular light calculation
		vec3 reflectDir = reflect(-lightDirection, norm);
		float lightDistance = length(lights[i].position - FragPos);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specularColor = lights[i].specular * spec * material.specular;
	
		// Add the results to the resulting light color
		resColor += (ambientColor + diffuseColor + specularColor)*calculateAttenuation(lightDistance, lights[i]);
	}
	
	// Sample texture to get object color.
	vec4 objColor = texture(textureUnit, texCoords);
	
	// Output color.
	fragColor = objColor * vec4(resColor, 1.0f);
}

//=============================================================================
// End of file
//=============================================================================