/**
 * @file	RawModel.h
 * @Author	Joakim Bertils
 * @date	2017-02-12
 * @brief	Base Class for representing a model in openGL.
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ShaderProgram.h"

class RawModel
{
public:
	explicit RawModel(const char* fileName);

	virtual void draw();

	virtual ~RawModel();
private:

	VertexArrayObject vao{};
	VertexBufferObject vertexPositions{ GL_ARRAY_BUFFER };
	VertexBufferObject vertexNormals{ GL_ARRAY_BUFFER };
	VertexBufferObject textureCoordinates{ GL_ARRAY_BUFFER };

	VertexBufferObject indexBuffer{ GL_ELEMENT_ARRAY_BUFFER };
};