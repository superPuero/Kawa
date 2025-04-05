#include "VAO.h"
#include <iostream>
namespace kawa
{

	VAO::VAO()
	{}

	void VAO::create()
	{
		glGenVertexArrays(1, &id);
	}

	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &id);
		unbind();
	}

	GLuint VAO::get_id()
	{
		return id;
	}


	void VAO::set_attrib_ptr(GLuint index, GLint size, GLenum type, GLboolean normilized, GLsizei stride, const void* ptr)
	{
		glVertexAttribPointer(index, size, type, normilized, stride, ptr);
	}

	void VAO::set_attrib_int_ptr(GLuint index, GLint size, GLenum type, GLboolean normilized, GLsizei stride, const void* ptr)
	{
		glVertexAttribIPointer(index, size, type, stride, ptr);
	}

	void VAO::enable_attrib_array(GLuint index)
	{
		glEnableVertexAttribArray(index);
	}

	void VAO::bind()
	{
		glBindVertexArray(id);
	}

	void VAO::unbind()
	{
		glBindVertexArray(0);
	}
}
