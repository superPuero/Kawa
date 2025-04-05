#pragma once
#include "glad/glad.h"

namespace kawa
{

	class VAO
	{
	public:
		VAO();
		~VAO();
		void create();

	public:
		GLuint get_id();
		void set_attrib_ptr(GLuint index, GLint size, GLenum type, GLboolean normilized, GLsizei stride, const void* ptr);
		void set_attrib_int_ptr(GLuint index, GLint size, GLenum type, GLboolean normilized, GLsizei stride, const void* ptr);
		void enable_attrib_array(GLuint index);
		void bind();
		void unbind();

	private:
		GLuint id;
	};
}

