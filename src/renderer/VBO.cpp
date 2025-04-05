#include "VBO.h"
namespace kawa
{

	VBO::VBO() {}

	VBO::~VBO()
	{
		glDeleteBuffers(1, &id);
		unbind();
	}

	void VBO::create()
	{
		glGenBuffers(1, &id);
	}

	void VBO::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void VBO::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VBO::data(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
	{
		glBufferData(target, size, data, usage);
	}
}
