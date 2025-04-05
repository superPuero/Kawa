#pragma once
#include "glad/glad.h"
namespace kawa
{

	class EBO
	{
	public:
		EBO();
		~EBO();
		void create();

	public:
		void bind();
		void unbind();
		void data(GLsizeiptr size, GLsizei ammount, void* data);
		void draw();

	private:
		GLuint _index;
		GLsizei _size;
		GLsizei _ammount;

	};

}
