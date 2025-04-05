#pragma once
#include "glad/glad.h"

namespace kawa
{

	class VBO
	{
	public:
		VBO();
		~VBO();
		void create();

	public:
		void bind();
		void unbind();
		void data(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

	private:
		GLuint id;
	};
}
