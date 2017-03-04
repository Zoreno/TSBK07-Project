/**
 * @file	VertexArrayObject.h
 * @Author	Joakim Bertils
 * @date	2017-02-12
 * @brief	VAO abstaction for mindless memory allocation and freeing.
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	// Implement when needed
	explicit VertexArrayObject(const VertexArrayObject&) = delete;
	explicit VertexArrayObject(VertexArrayObject&&) = delete;
	const VertexArrayObject& operator=(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(VertexArrayObject&&) = delete;

	void bind();
	void unbind();

	GLuint getHandle() const;
private:

	GLuint vao;
};