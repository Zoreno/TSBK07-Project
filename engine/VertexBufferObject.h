/**
 * @file	VertexBufferObject.h
 * @Author	Joakim Bertils
 * @date	2017-02-12
 * @brief	VBO abstaction for mindless memory allocation and freeing.
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class VertexBufferObject
{
public:
	explicit VertexBufferObject(GLenum target);
	~VertexBufferObject();

	explicit VertexBufferObject(const VertexBufferObject&) = delete;
	explicit VertexBufferObject(VertexBufferObject&&) = delete;
	const VertexBufferObject& operator=(const VertexBufferObject&) = delete;
	VertexBufferObject& operator=(VertexBufferObject&&) = delete;

	void bind();
	void unbind();

	void storeData(GLuint size, const void* data, GLenum usage);
	void setupVertexAttribPointer(GLuint location, GLuint elementSize);

	GLuint getHandle() const;
	GLenum getTarget() const;
	GLuint getSize() const;
private:
	GLuint vbo;
	GLenum target;
	GLuint size;
};