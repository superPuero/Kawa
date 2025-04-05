#include "EBO.h"
#include <iostream>
namespace kawa
{

	EBO::EBO()
	{
	}

	EBO::~EBO()
	{
		glDeleteBuffers(1, &_index);
		unbind();
	}

	void EBO::create()
	{
		glGenBuffers(1, &_index);
	}

	void EBO::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index);
	}

	void EBO::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void EBO::data(GLsizeiptr size, GLsizei ammount, void* data)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * ammount, data, GL_STATIC_DRAW);
		_size = size;
		_ammount = ammount;
	}

	void EBO::draw()
	{
		glDrawElements(GL_TRIANGLES, _ammount, GL_UNSIGNED_INT, 0);
	}
}
